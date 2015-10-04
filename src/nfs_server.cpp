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

#define MAX_WIDTH 20

std::string format_size(long size) {
	std::ostringstream formatted;

	if (size < 1024)
		formatted << size << "B";
	else if (size < 1024 * 1024)
		formatted << size / 1024 << "KB";
	else if (size < 1024 * 1024 * 1024)
		formatted << size / 1024 / 1024 << "MB";

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

      files << std::left << std::setw(MAX_WIDTH) << de->d_name << format_size(st.st_size) << std::endl;
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
	// std::cout << result;
	// sprintf(result, "%s%s", str, str);

	// free(result);
	/*
	 * insert server code here
	 */

	return &result;
}

int *
create_1_svc(char *filename,  struct svc_req *rqstp)
{
	static int  result = NO_ERROR;
	if (!is_filename_valid(std::string(filename)))
		result = E_FILENAME_INVALID;
	if (access(filename, F_OK) == 0)
    result = E_FILE_EXISTS;

	if (result == NO_ERROR) {
		mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
		open(filename, O_CREAT | O_APPEND, mode);
		std::cout << "Created '" << filename << "'\n";
	}

	return &result;
}

int *
delete_1_svc(char *filename,  struct svc_req *rqstp)
{
	//TODO DRY
	//TODO UDP
	static int  result = NO_ERROR;
	if (!is_filename_valid(std::string(filename)))
		result = E_FILENAME_INVALID;
	if (access(filename, F_OK) != 0)
    result = E_FILE_NOT_EXISTS;

	if (result == NO_ERROR) {
		remove(filename);
		std::cout << "Deleted '" << filename << "'\n";
	}
	/*
	 * insert server code here
	 */

	return &result;
}
