/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */
#include <dirent.h>
#include <string>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <sys/stat.h>
#include <fcntl.h>

#include "nfs.h"

std::string format_size(long size) {
	std::ostringstream formatted;

	if (size < 1024)
		formatted << size << " B";
	else if (size < 1024 * 1024)
		formatted << size / 1024 << " KB";
	else if (size < 1024 * 1024 * 1024)
		formatted << size / 1024 / 1024 << " MB";

	return formatted.str();
}

bool is_filename_valid(std::string filename) {
	std::string dotdot = "..";
	if (filename.find(dotdot) != std::string::npos)
		return false;
	return true;
}

char* ls_dir() {
	// const char* path = argc <= 1 ? "." : argv[1];
	const char* path = ".";
	std::ostringstream files;

  DIR* d = opendir(path);
  if (d == NULL) return NULL;

	struct stat st;

  for(struct dirent *de = NULL; (de = readdir(d)) != NULL; ) {
    if (de->d_type == DT_REG) {
	    stat(de->d_name, &st);

      files << std::left << std::setw(MAX_LENGTH) << de->d_name << format_size(st.st_size) << std::endl;
		}
  }

  closedir(d);
	// std::cout << files.str() << std::endl;
	char *cstr = new char[files.str().length() + 1];
	strncpy(cstr, files.str().c_str(), files.str().length());
	cstr[files.str().length()] = '\0';
	return cstr;
}

char ** ls_1_svc(char *str,  struct svc_req *rqstp)
{
	static char * result;
	std::cout << "Listed files\n";
	result = ls_dir();

	return &result;
}

int *
create_1_svc(char *filename,  struct svc_req *rqstp)
{
	static int result;

	std::cout << "Received request to create a file with name '" << filename << "'\n";
	result = NO_ERROR;
	if (!is_filename_valid(std::string(filename)))
		result = E_FILENAME_INVALID;
	if (access(filename, F_OK) == 0)
    result = E_FILE_EXISTS;

	if (result != E_FILENAME_INVALID) {
		fopen(filename, "w");
		std::cout << "Created/truncated '" << filename << "'\n";
	}
	return &result;
}

int *
delete_1_svc(char *filename,  struct svc_req *rqstp)
{
	static int result;
	//TODO DRY, result = NO_ERROR;?
	//TODO UDP
	std::cout << "Received request to delete a file with name '" << filename << "'\n";
	result = NO_ERROR;
	if (!is_filename_valid(std::string(filename)))
		result = E_FILENAME_INVALID;
	if (access(filename, F_OK) != 0)
    result = E_FILE_NOT_EXISTS;

	if (result == NO_ERROR) {
		remove(filename);
		std::cout << "Deleted '" << filename << "'\n";
	}
	return &result;
}

chunk *
retrieve_file_1_svc(request req,  struct svc_req *rqstp)
{
	static chunk ch;
	ch.status = NO_ERROR;

	FILE *file;
  file = fopen(req.filename, "r");

	if (access(req.filename, F_OK) != 0)
    ch.status = E_FILE_NOT_EXISTS;

	if (req.offset == 0)
		std::cout << "Requested file '" << req.filename << "'\n";

	if (ch.status == NO_ERROR) {
		fseek(file, req.offset, SEEK_SET);
		ch.data.data_val = (char *) malloc(DATA_LENGTH * sizeof(char));
		ch.data.data_len = fread(ch.data.data_val, 1, DATA_LENGTH, file);
		ch.dest_offset = req.offset;
		ch.filename = req.filename;
	  fclose(file);
	}

	return &ch;
}

int *
send_file_1_svc(chunk ch,  struct svc_req *rqstp)
{
	static int result;
  result = NO_ERROR;

	FILE *file;
  file = fopen(ch.filename, "a");
  if (file == NULL) {
      result = errno;
      return &result;
  }

	fseek(file, ch.dest_offset, SEEK_SET);
  fwrite(ch.data.data_val, 1, ch.data.data_len, file);
  fclose(file);

	if (ch.dest_offset == 0)
		std::cout << "Received file '" << ch.filename << "'\n";

	return &result;
}

int *
mkdir_1_svc(char *dirname,  struct svc_req *rqstp)
{
	static int result;

	std::cout << "Received request to create a directory with name '" << dirname << "'\n";
	result = NO_ERROR;
	if (!is_filename_valid(std::string(dirname)))
		result = E_FILENAME_INVALID;
	if (access(dirname, F_OK) == 0)
    result = E_FILE_EXISTS;

	if (result != E_FILENAME_INVALID) {
		mkdir(dirname, 0700);
		std::cout << "Created '" << dirname << "'\n";
	}
	return &result;
}
