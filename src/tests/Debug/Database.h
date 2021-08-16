#ifndef __DATABASE_H__
#define __DATABASE_H__

#include<stdio.h>
#include<stdlib.h>
#include<sqlite3.h>
#include<string>
#include<iostream>
#include<cstdlib>
#include<chrono>
#include<ctime>
#include<thread>

//db file path
#ifndef DB_PATH
#define DB_PATH (std::string(std::getenv("XEYES_BUILD"))+std::string("/xeyes.db")).c_str()
#endif
namespace xeyes {
    class Database {
        public:
            const char* dir; //db file path

            //default constructor
            Database() {}

            //assignment constructor
            Database(const char* path)
            : dir(path)
            {
            }

            //connect to database
            int createDB(const char* dir);
            //create a table
            int createTable(const char* dir);
            //delete entry from table
            //static int deleteData(const char* dir);
            //insert entry to table
            int insertData(const char* dir, int cam_id, std::string cam_name, std::string start, std::string end);
            //update entry in table
            int updateData(const char* dir, int id, std::string end);
            //fetch entry from table
            int selectData(const char* dir);
            //delete all entries from table
            int clearTable(const char* dir);
            //callbacks
            static int callback(void* data, int argc, char** argv, char** azColName);

    };
}
#endif