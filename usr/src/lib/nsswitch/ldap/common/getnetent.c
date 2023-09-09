/*
 * Copyright (c) 1999 by Sun Microsystems, Inc.
 * All rights reserved.
 */

#pragma ident   "@(#)getnetent.c 1.2     99/10/22 SMI"

#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "ldap_common.h"

/* networks attributes filters */
#define	_N_NAME		"cn"
#define	_N_NETWORK	"ipnetworknumber"
#define	_F_GETNETBYNAME	"(&(objectClass=ipNetwork)(cn=%s))"
#define	_F_GETNETBYADDR	"(&(objectClass=ipNetwork)(ipNetworkNumber=%s))"

static const char *networks_attrs[] = {
	_N_NAME,
	_N_NETWORK,
	(char *)NULL
};

/*
 * _nss_ldap_networks2ent is the data marshaling method for the networks
 * getXbyY * (e.g., getbyname(), getbyaddr(), getnetent() backend processes.
 * This method is called after a successful ldap search has been performed.
 * This method will parse the ldap search values into struct netent =
 * argp->buf.buffer which the frontend process expects. Three error conditions
 * are expected and returned to nsswitch.
 */

static int
_nss_ldap_networks2ent(ldap_backend_ptr be, nss_XbyY_args_t *argp)
{
	int		i, j;
	int		nss_result;
	int		buflen = (int)0;
	int		firstime = (int)1;
	unsigned long	len = 0L;
	char		**mp, addrstr[16];
	char		*buffer = (char *)NULL;
	char		*ceiling = (char *)NULL;
	struct netent	*ntk = (struct netent *)NULL;
	ns_ldap_result_t	*result = be->result;
	ns_ldap_attr_t	*attrptr;

	buffer = argp->buf.buffer;
	buflen = (size_t)argp->buf.buflen;
	if (!argp->buf.result) {
		nss_result = (int)NSS_STR_PARSE_ERANGE;
		goto result_net2ent;
	}
	ntk = (struct netent *)argp->buf.result;
	ceiling = buffer + buflen;

	nss_result = (int)NSS_STR_PARSE_SUCCESS;
	(void) memset(argp->buf.buffer, 0, buflen);

	attrptr = getattr(result, 0);
	if (attrptr == NULL) {
		nss_result = (int)NSS_STR_PARSE_PARSE;
		goto result_net2ent;
	}

	for (i = 0; i < result->entry->attr_count; i++) {
		attrptr = getattr(result, i);
		if (attrptr == NULL) {
			nss_result = (int)NSS_STR_PARSE_PARSE;
			goto result_net2ent;
		}
		if (strcasecmp(attrptr->attrname, _N_NAME) == 0) {
			for (j = 0; j < attrptr->value_count; j++) {
				if (firstime) {
					/* official name of net */
					len = strlen(attrptr->attrvalue[j]);
					if (len < 1 ||
					    (attrptr->attrvalue[j] == '\0')) {
						nss_result =
						    (int)NSS_STR_PARSE_PARSE;
						goto result_net2ent;
					}
					ntk->n_name = buffer;
					buffer += len + 1;
					if (buffer >= ceiling) {
						nss_result =
						    (int)NSS_STR_PARSE_ERANGE;
						goto result_net2ent;
					}
					(void) strcpy(ntk->n_name,
						attrptr->attrvalue[j]);
					/* alias list */
					mp = ntk->n_aliases =
						(char **)ROUND_UP(buffer,
						sizeof (char **));
					buffer = (char *)ntk->n_aliases +
						sizeof (char *) *
						(attrptr->value_count + 1);
					buffer = (char *)ROUND_UP(buffer,
						sizeof (char **));
					if (buffer >= ceiling) {
						nss_result =
						    (int)NSS_STR_PARSE_ERANGE;
						goto result_net2ent;
					}
					firstime = (int)0;
				}
				/* alias list */
				len = strlen(attrptr->attrvalue[j]);
				if (len < 1 ||
				    (attrptr->attrvalue[j] == '\0')) {
					nss_result = (int)NSS_STR_PARSE_PARSE;
					goto result_net2ent;
				}
				*mp = buffer;
				buffer += len + 1;
				if (buffer >= ceiling) {
					nss_result = (int)NSS_STR_PARSE_ERANGE;
					goto result_net2ent;
				}
				(void) strcpy(*mp++, attrptr->attrvalue[j]);
				continue;
			}
		}
		if (strcasecmp(attrptr->attrname, _N_NETWORK) == 0) {
			len = strlen(attrptr->attrvalue[0]);
			if (len == 0 || (attrptr->attrvalue[0] == '\0')) {
				nss_result = (int)NSS_STR_PARSE_PARSE;
				goto result_net2ent;
			}
			if ((ntk->n_net = (in_addr_t)
			    inet_network(attrptr->attrvalue[0])) ==
			    (in_addr_t) - 1) {
				nss_result = (int)NSS_STR_PARSE_PARSE;
				goto result_net2ent;
			}
			(void) strcpy(addrstr, attrptr->attrvalue[0]);
			continue;
		}
	}
	ntk->n_addrtype = AF_INET;

#ifdef DEBUG
	(void) fprintf(stdout, "\n[getnetent.c: _nss_ldap_networks2ent]\n");
	(void) fprintf(stdout, "        n_name: [%s]\n", ntk->n_name);
	if (mp != NULL) {
		for (mp = ntk->n_aliases; *mp != NULL; mp++)
			(void) fprintf(stdout, "     n_aliases: [%s]\n", *mp);
	}
	if (ntk->n_addrtype == AF_INET)
		(void) fprintf(stdout, "    n_addrtype: [AF_INET]\n");
	else
		(void) fprintf(stdout, "    n_addrtype: [%d]\n",
			    ntk->n_addrtype);
	(void) fprintf(stdout, "         n_net: [%s]\n", addrstr);
#endif DEBUG

result_net2ent:

	(void) __ns_ldap_freeResult(&be->result);
	return ((int)nss_result);
}


/*
 * Takes an unsigned integer in host order, and returns a printable
 * string for it as a network number.  To allow for the possibility of
 * naming subnets, only trailing dot-zeros are truncated.
 */

static int nettoa(int anet, char *buf, int buflen)
{
	int		addr;
	char		*p;
	struct in_addr	in;

	if (buf == 0)
		return ((int)1);

	in = inet_makeaddr(anet, INADDR_ANY);
	addr = in.s_addr;
	(void) strncpy(buf, inet_ntoa(in), buflen);
	if ((IN_CLASSA_HOST & htonl(addr)) == 0) {
		p = strchr(buf, '.');
		if (p == NULL)
			return ((int)1);
		*p = 0;
	} else if ((IN_CLASSB_HOST & htonl(addr)) == 0) {
		p = strchr(buf, '.');
		if (p == NULL)
			return ((int)1);
		p = strchr(p + 1, '.');
		if (p == NULL)
			return ((int)1);
		*p = 0;
	} else if ((IN_CLASSC_HOST & htonl(addr)) == 0) {
		p = strrchr(buf, '.');
		if (p == NULL)
			return ((int)1);
		*p = 0;
	}

	return ((int)0);
}


/*
 * getbyname gets a network entry by name. This function constructs an
 * ldap search filter using the network name invocation parameter and the
 * getnetbyname search filter defined. Once the filter is constructed, we
 * search for a matching entry and marshal the data results into struct
 * netent for the frontend process. The function _nss_ldap_networks2ent
 * performs the data marshaling.
 */

static nss_status_t
getbyname(ldap_backend_ptr be, void *a)
{
	nss_XbyY_args_t	*argp = (nss_XbyY_args_t *)a;
	char		searchfilter[SEARCHFILTERLEN];

	if (snprintf(searchfilter, SEARCHFILTERLEN,
		_F_GETNETBYNAME, argp->key.name) < 0)
		return ((nss_status_t)NSS_NOTFOUND);

	return ((nss_status_t)_nss_ldap_lookup(be, argp,
		_NETWORKS, searchfilter, NULL));
}


/*
 * getbyaddr gets a network entry by ip address. This function constructs an
 * ldap search filter using the name invocation parameter and the getnetbyaddr
 * search filter defined. Once the filter is constructed, we search for a
 * matching entry and marshal the data results into struct netent for the
 * frontend process. The function _nss_ldap_networks2ent performs the data
 * marshaling.
 */

static nss_status_t
getbyaddr(ldap_backend_ptr be, void *a)
{
	nss_XbyY_args_t	*argp = (nss_XbyY_args_t *)a;
	char		addrstr[16];
	char		searchfilter[SEARCHFILTERLEN];

	if (nettoa((int)argp->key.netaddr.net, addrstr, 16) != 0)
		return ((nss_status_t)NSS_UNAVAIL);

	if (snprintf(searchfilter, SEARCHFILTERLEN,
		_F_GETNETBYADDR, addrstr) < 0)
		return ((nss_status_t)NSS_NOTFOUND);

	return ((nss_status_t)_nss_ldap_lookup(be, argp,
		_NETWORKS, searchfilter, NULL));
}

static ldap_backend_op_t net_ops[] = {
	_nss_ldap_destr,
	_nss_ldap_endent,
	_nss_ldap_setent,
	_nss_ldap_getent,
	getbyname,
	getbyaddr
};


/*
 * _nss_ldap_networks_constr is where life begins. This function calls the
 * generic ldap constructor function to define and build the abstract data
 * types required to support ldap operations.
 */

/*ARGSUSED0*/
nss_backend_t *
_nss_ldap_networks_constr(const char *dummy1, const char *dummy2,
			const char *dummy3)
{

	return ((nss_backend_t *)_nss_ldap_constr(net_ops,
		sizeof (net_ops)/sizeof (net_ops[0]), _NETWORKS,
		networks_attrs, _nss_ldap_networks2ent));
}
