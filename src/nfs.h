/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _NFS_H_RPCGEN
#define _NFS_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


#define NFS_PROGRAM 0x21000001
#define NFS_VERSION_1 1

#if defined(__STDC__) || defined(__cplusplus)
#define ls 1
extern  int * ls_1(int , CLIENT *);
extern  int * ls_1_svc(int , struct svc_req *);
extern int nfs_program_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define ls 1
extern  int * ls_1();
extern  int * ls_1_svc();
extern int nfs_program_1_freeresult ();
#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_NFS_H_RPCGEN */
