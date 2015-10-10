# nfs_sunrpc

## TODO
 - OPERATIONS
   - ~~create empty file~~
   - ~~write to file~~
   - ~~read from file~~
   - ~~delete file~~
   - ~~prompt~~
   - ~~log errors&dones on server~~
   - ~~set root directory for server~~
   - cd
   - directiories
   - mutex at delete/read/write -> server is single-threaded?

## ADDITIONAL TODOS
 - replication

send(name):
  if (file(name) is file):
    sendfile(name)
  else:
    mkdir dir
    foreach(file in dir):
      sendfile(file)

retrieve(name):
  // if (file return file, if dir return list of files) (info in chunk.status?)
  // TODO list is already returned by ls
  if (status == FILE)
    normally_proceed as with file
  if (status == DIR)
    create dir locally
    read list of all dirs to string
    foreach (filename in list)
      retrieve(filename)
