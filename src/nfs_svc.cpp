/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "nfs.h"
#include <stdio.h>
#include <stdlib.h>
#include <rpc/pmap_clnt.h>
#include <string.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#ifndef SIG_PF
#define SIG_PF void(*)(int)
#endif

static char **
_ls_1 (char * *argp, struct svc_req *rqstp)
{
	return (ls_1_svc(*argp, rqstp));
}

static int *
_create_1 (char * *argp, struct svc_req *rqstp)
{
	return (create_1_svc(*argp, rqstp));
}

static int *
_delete_1 (char * *argp, struct svc_req *rqstp)
{
	return (delete_1_svc(*argp, rqstp));
}

static chunk *
_retrieve_file_1 (request  *argp, struct svc_req *rqstp)
{
	return (retrieve_file_1_svc(*argp, rqstp));
}

static int *
_send_file_1 (chunk  *argp, struct svc_req *rqstp)
{
	return (send_file_1_svc(*argp, rqstp));
}

static int *
_mkdir_1 (char * *argp, struct svc_req *rqstp)
{
	return (mkdir_1_svc(*argp, rqstp));
}

static void
nfs_program_1(struct svc_req *rqstp, register SVCXPRT *transp)
{
	union {
		char *ls_1_arg;
		char *create_1_arg;
		char *delete_1_arg;
		request retrieve_file_1_arg;
		chunk send_file_1_arg;
	} argument;
	char *result;
	xdrproc_t _xdr_argument, _xdr_result;
	char *(*local)(char *, struct svc_req *);

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
		return;

	case ls:
		_xdr_argument = (xdrproc_t) xdr_wrapstring;
		_xdr_result = (xdrproc_t) xdr_wrapstring;
		local = (char *(*)(char *, struct svc_req *)) _ls_1;
		break;

	case create:
		_xdr_argument = (xdrproc_t) xdr_wrapstring;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (char *(*)(char *, struct svc_req *)) _create_1;
		break;

	case delete:
		_xdr_argument = (xdrproc_t) xdr_wrapstring;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (char *(*)(char *, struct svc_req *)) _delete_1;
		break;

	case retrieve_file:
		_xdr_argument = (xdrproc_t) xdr_request;
		_xdr_result = (xdrproc_t) xdr_chunk;
		local = (char *(*)(char *, struct svc_req *)) _retrieve_file_1;
		break;

	case send_file:
		_xdr_argument = (xdrproc_t) xdr_chunk;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (char *(*)(char *, struct svc_req *)) _send_file_1;
		break;

	case mk_dir:
		_xdr_argument = (xdrproc_t) xdr_wrapstring;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (char *(*)(char *, struct svc_req *)) _mkdir_1;
		break;

	default:
		svcerr_noproc (transp);
		return;
	}
	memset ((char *)&argument, 0, sizeof (argument));
	if (!svc_getargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		svcerr_decode (transp);
		return;
	}
	result = (*local)((char *)&argument, rqstp);
	if (result != NULL && !svc_sendreply(transp, (xdrproc_t) _xdr_result, result)) {
		svcerr_systemerr (transp);
	}
	if (!svc_freeargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		fprintf (stderr, "%s", "unable to free arguments");
		exit (1);
	}
	return;
}

int
main (int argc, char **argv)
{
	if (argc == 2)
		chdir(argv[1]);

	SVCXPRT *transp;

	pmap_unset (NFS_PROGRAM, NFS_VERSION_1);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create udp service.");
		exit(1);
	}
	if (!svc_register(transp, NFS_PROGRAM, NFS_VERSION_1, nfs_program_1, IPPROTO_UDP)) {
		fprintf (stderr, "%s", "unable to register (NFS_PROGRAM, NFS_VERSION_1, udp).");
		exit(1);
	}

	transp = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create tcp service.");
		exit(1);
	}
	if (!svc_register(transp, NFS_PROGRAM, NFS_VERSION_1, nfs_program_1, IPPROTO_TCP)) {
		fprintf (stderr, "%s", "unable to register (NFS_PROGRAM, NFS_VERSION_1, tcp).");
		exit(1);
	}

	svc_run ();
	fprintf (stderr, "%s", "svc_run returned");
	exit (1);
	/* NOTREACHED */
}
