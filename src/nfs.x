const FILENAME_LENGTH = 128;
const DATA_LENGTH = 1024;

struct request {
    string filename<FILENAME_LENGTH>;
    int start;
};

struct chunk {
    string filename<FILENAME_LENGTH>;
    opaque data<DATA_LENGTH>;
    int size;
    int dest_offset;
};
typedef struct chunk chunk;

program NFS_PROGRAM {
  version NFS_VERSION_1 {
    string ls(string str) = 1;
    int create(string filename) = 2;
    int delete(string filename) = 3;
    chunk retrieve_file(request*) = 4;
    int send_file(chunk *) = 5;
    int mkdir(string dirname) = 6;
  } = 1;
} = 0x21000001;
