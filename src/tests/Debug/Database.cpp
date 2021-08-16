#include "Database.h"

using namespace std;

int xeyes::Database::createDB(const char* dir) {
    sqlite3* db;
    int rc; //return code
    
    rc = sqlite3_open(dir, &db);

    if( rc ) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return(0);
    } else {
        fprintf(stderr, "Opened Database Successfully!\n");
    }
    sqlite3_close(db);

    return 0;
}

int xeyes::Database::createTable(const char* dir) {
    sqlite3* db;
    
    std::string sql = "CREATE TABLE IF NOT EXISTS DETECTIONS("
        "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
        "CAM_ID     INT NOT NULL, "  
        "CAM_NAME   TEXT NOT NULL, "  
        "START      TEXT NOT NULL, "  
        "END        TEXT NOT NULL );";
    
    try
    {
        int rc = 0;
        rc = sqlite3_open(dir, &db);

        char* errorMsg;
        rc = sqlite3_exec(db, sql.c_str(), NULL, 0, &errorMsg);

        if (rc != SQLITE_OK) {
            cerr << "Error in Create Table!" << endl;
            sqlite3_free(errorMsg);
        } else {
            cout << "Table Created Succesfully!" << endl;
        }
        sqlite3_close(db);
    } 
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;    
}

int xeyes::Database::insertData(const char* dir, int cam_id, std::string cam_name, std::string start, std::string end) {
    sqlite3* db;
    char* errorMsg;

    /*
    std::string sql = std::string("INSERT INTO DETECTIONS (CAM_ID, CAM_NAME, START, END) VALUES("
     + std::to_string(cam_id) + ", '"
     + cam_name + "', '"
     + start + "', '"
     + end + "'"
    ") ON CONFLICT(START) DO UPDATE SET END='" + end + "'"";");
    */

    std::string sql = std::string("INSERT INTO DETECTIONS (CAM_ID, CAM_NAME, START, END) VALUES("
     + std::to_string(cam_id) + ", '"
     + cam_name + "', '"
     + start + "', '"
     + end + "'"
    ");");

    int rc = sqlite3_open(dir, &db);

    rc = sqlite3_exec(db, sql.c_str(), NULL, 0, &errorMsg);
    if (rc != SQLITE_OK) {
        cout << errorMsg << endl;
        sqlite3_free(errorMsg);
    } else {
        cout << "Records Created Succesfully!" << endl;
    }

    return 0;
}

int xeyes::Database::selectData(const char* dir) {
    sqlite3* db;
    char* errorMsg;

    std::string sql = "SELECT * FROM DETECTIONS;";

    int rc = sqlite3_open(dir, &db);
    rc = sqlite3_exec(db, sql.c_str(), callback, NULL, &errorMsg);

    if(rc != SQLITE_OK) {
        cerr << "Error in Select!" << endl;
        sqlite3_free(errorMsg);
    } else {
        cout << "Records Selected Succesfully!" << endl;
    }

    return 0;
}

int xeyes::Database::updateData(const char* dir, int id, std::string end) {
    sqlite3* db;
    char* errorMsg;

    int rc = sqlite3_open(dir, &db);

    std::string sql("UPDATE DETECTIONS set END = "
        + end + "WHERE ID="
        + "'" + std::to_string(id) + "'" + ";" \
        "SELECT * from DETECTIONS;");

    rc = sqlite3_exec(db, sql.c_str(), NULL, 0, &errorMsg);
    if (rc != SQLITE_OK) {
        cout << errorMsg << endl;
        cerr << "Error Update" << endl;
        sqlite3_free(errorMsg);
    } else {
        cout << "Records Updated Succesfully!" << endl;
    }

    return 0;
}

int xeyes::Database::clearTable(const char* dir) {
    sqlite3* db;
    char* errorMsg;

    int rc = sqlite3_open(dir, &db);

    std::string sql("DELETE FROM DETECTIONS;");

    rc = sqlite3_exec(db, sql.c_str(), NULL, 0, &errorMsg);
     if (rc != SQLITE_OK) {
        cout << errorMsg << endl;
        cerr << "Error Clear Table" << endl;
        sqlite3_free(errorMsg);
    } else {
        cout << "Table Cleared Succesfully!" << endl;
    }

    return 0;
}

int xeyes::Database::callback(void* data, int argc, char** argv, char** azColName) {
    for(int i=0; i<argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");

    return 0;
}
