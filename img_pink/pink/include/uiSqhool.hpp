
#ifndef UISQHOOL_H
#define UISQHOOL_H

#include <string>
#include <vector>

#include "ui_pink_types.hpp"


#define sqlite3com(command, errormessage)				      \
 if (SQLITE_OK!=command){                                             \
   std::cout << "sqlite says: " << sqlite3_errmsg(database) << std::endl;       \
   pink_error(errormessage);                                               \
 };

typedef char *   pchar;
typedef char ** ppchar;

typedef sqlite3 * psqlite3;

typedef sqlite3_blob *   psqlite3_blob;
typedef sqlite3_blob ** ppsqlite3_blob;

typedef sqlite3_stmt *   psqlite3_stmt;
typedef sqlite3_stmt ** ppsqlite3_stmt;



namespace pink{

  void set_dimensions(const types::vint & dim, index_t & x, index_t & y, index_t & z, index_t & t);
  boost::shared_ptr<types::vint> get_dimensions(const index_t x, const index_t y, const index_t z, const index_t t);

  
/**
 * class uiSqhool
 * This is an object to create and read the UJIF format. The UJIF is an SQLite
 * database file suied to store multi dimensional images. Each file can contain
 * arbitrary many images with arbitrary dimension.
 * Note: as of version 4.1.1.0 of the format up to 4D float32 or byte grayscale
 * images are supported.
 */
  class uiSqhool
  {
  public:
    
    uiSqhool ( );
    virtual ~uiSqhool ( );
    
    uiSqhool (const std::string & filename, const std::string & creator = "", const std::string & description = "", bool create = false );
    void init (const std::string & filename, const std::string & creator = "", const std::string & description = "", bool create = false );

    template <class image_type>
    boost::shared_ptr<image_type> get_image ( int ID = -1 );

    template <class image_type> 
    void insert_image ( const image_type & image, const std::string & relative_path, std::string full_path );

    void set_comment (int ID, const std::string & comment );
    void set_log (int ID, const std::string & log );
    void set_command (int ID, const std::string & command );
    void set_substitution (int ID_final_image, int ID_source_image, const std::string & name_source_in_command );
    std::string get_image_type( int ID = -1 );

    boost::shared_ptr<std::vector<std::string> > get_commands (int ID = -1 );
    boost::shared_ptr<types::vint> list_images ( );

  private:

    psqlite3 database;
    std::string filename;
    int lock;

    static int callback (void * NotUsed, int argc, ppchar argv, ppchar azColName );
    int  get_SQL_value(const std::stringstream & command);
    void sql_execute(const std::stringstream & command, std::string error_message="couldn't execute the command");
    void insert_blob(void * data, const int size, const std::stringstream & ss);
    boost::shared_ptr<types::vint> read_data_details( char ** results, int pos , int d );
    void touch(); // update the last-modified stamp in the fileinfo table.
    boost::shared_ptr<std::string> get_command( int ID );
    boost::shared_ptr<types::vint> get_dependencies( const types::vint & IDs );
    void init_attributes ( ) ;


  }; /* class uiSqhool */

  extern std::string uiCreateFile;
  
  template <class image_type>  
  boost::shared_ptr<image_type> uiSqhool::get_image( int ID ){
    types::progress_bar sentinel;
    sentinel.start();
    
    char ** results;
    int row;
    int column;
    char * errmsg;
    
    // reading the dimensions of the image from the file
    std::stringstream ss;
    ss << "select size_x,size_y,size_z,size_t,data_type from images where id=" << ID << ";";
    
    sqlite3_get_table(
      database,          // sqlite3 *db,          /* An open database */
      ss.str().c_str(),  // const char *zSql,     /* SQL to be evaluated */
      &results,          // char ***pazResult,    /* Results of the query */
      &row,              // int *pnRow,           /* Number of result rows written here */
      &column,           // int *pnColumn,        /* Number of result columns written here */
      &errmsg            // char **pzErrmsg       /* Error msg written here */
      );
    
    int x,y,z,t;
    std::stringstream sstype;
    x=atoi(results[5+0]);
    y=atoi(results[5+1]);
    z=atoi(results[5+2]);
    t=atoi(results[5+3]);
    sstype << results[5+4];
    sqlite3_free_table(results);
    
    boost::shared_ptr<types::vint> dim = get_dimensions(x,y,z,t);
    int d = dim->size();
//   FOR (q,int(dim.size())){
//     std::cout << "dim[" << q << "]=" << dim[q] << std::endl;
//   };
    
    std::cout << "getting a '" << sstype.str() << "' image, dimension is " << d << "D (x=" << x << ", y=" << y << ", z=" << z << ", t=" << t << ")" << std::endl;
    
    boost::shared_ptr< image_type > result(new image_type());
    
    // getting the rowid's of the lines, that contain the image
    
    std::stringstream ss2;
    ss2 << "select rowid,pos_x,pos_y,pos_z,pos_t,size_x,size_y,size_z,size_t from datas where id=" << ID << ";";
    sqlite3com (sqlite3_get_table( database, ss2.str().c_str(), &results, &row, &column, &errmsg), 
	     "couldn't get the list of rowid s which containt the blobs");
    
    FOR(q, row){ // the ids are from 1 to row, that is q+1
      sqlite3_int64 iRow;
      boost::shared_ptr<types::vint> pos  = read_data_details( results, 9*(q+1)+1, d );
      boost::shared_ptr<types::vint> size = read_data_details( results, 9*(q+1)+5, d );
      
      FOR( w, int(size->size()) ) {
	std::cout << "(*size)[" << w << "]=" << (*size)[w] << std::endl;
      } /* FOR( w, int(size->size()) ) */
      
      iRow = atoi(results[9*(q+1)+0]);
      
      psqlite3_blob blob;
      
      sqlite3com( 
	sqlite3_blob_open( database, "main", "datas", "data", iRow, OPEN_BLOB_RO, &blob ),
	"couldn't open the blob from the datas table in the image file"
	);


      // testing if the type of the image to read is the same as 
      // image_type

      if (result->imtype()!=sstype.str()) 
      {
	pink_error("you are trying to read an image to an object of different type.");
      }; /* if */

      // this is the type of the image to read
      typedef typename image_type::pixel_type pixel_type;
      
      
      boost::shared_array<pixel_type> data (new pixel_type[size->prod()]);
      
      if (sqlite3_blob_bytes(blob) != int(sizeof(pixel_type)*size->prod()))
	pink_error("the size of the data is not corresponding with the size defined by the tags");
      
      
      sqlite3com( sqlite3_blob_read(blob, (void*)data.get(), sizeof(pixel_type)*size->prod(), 0), 
	       "the data is not read");
      
      result.reset( new image_type( *size, data ) );
      
      sqlite3com( sqlite3_blob_close(blob) , "couldn't close the blob");
      
    };
    
    sqlite3_free_table(results);
    
    std::cout << "Image read in " << sentinel.elapsedTime() << std::endl;
    return result;
  }
  

  template <class image_type>  
  void uiSqhool::insert_image ( const image_type & image, 
				const std::string & relative_path, 
				std::string full_path=""
    ) {
    //In a later time an image can be broken up in order to respect the 10E6 byte blob limit.
    
    types::progress_bar sentinel;
    sentinel.start();


    boost::shared_ptr<image_type> result( new image_type() );
    std::string type=""; /////!!!!!!!!!!!!!!!!!!!!!
    std::string data_type(result->imtype());

      


    std::cout << "image.get_size().size()=" << image.get_size().size() << std::endl;
    index_t x,y,z,t;
    //setting up dimensions
    set_dimensions(image.get_size(), x, y, z, t); // as 4 dimensions are supported
    
    std::cout << "inserting image with size: (x=" << x << ", y=" << y << ", z=" << z << ", t=" << t << ")" << std::endl;
    
    int new_id;
    
    {
      std::stringstream command;
      command << "select count(id) from images;";
      
      new_id = 1 + get_SQL_value( command );
      
      std::stringstream ss;     
      ss << 
	"insert into images (ID, size_x, size_y, size_z, size_t, type, full_path, relative_path, data_type) values (" 
	 << new_id << "," 
	 << x << "," 
	 << y << "," 
	 << z << "," 
	 << t <<  ",'"  
	 << type << "','" 
	 << full_path << "','" 
	 << relative_path << "','" 
	 << data_type << "');";
      sql_execute(ss);
    }
    
    //------------- the corresponding line inserted into "images" table --------------
    
    //------------- inserting the data into the "datas" table ------------------------
    
    {
      
      std::stringstream ss; 
      ss << "insert into datas (ID, data, pos_x, pos_y, pos_z, pos_t, size_x, size_y, size_z, size_t) values ("
	 << new_id 
	 << ","
	 << "?" << // the data will be compiled here
	",0,0,0,0,"
	 << x << "," 
	 << y << "," 
	 << z << "," 
	 << t << ");";

      typedef typename image_type::pixel_type pixel_type;

      boost::shared_array<pixel_type> data = image.get_pixels();
      insert_blob((void*)data.get(), image.get_size().prod()*sizeof(char), ss);
      
    };
    
    std::cout << "image inserted in " << sentinel.elapsedTime() << " with id = " << new_id << std::endl;
    touch();
  }
  
  
  
  
  
  
  
  
  
  
  





























} /* namespace pink */

#undef error
#endif // UISQHOOL_H
//LuM end of file
