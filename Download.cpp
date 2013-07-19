//Jessica Milne 4/3/2010
////////////////////////////////////////////////////////////////
/* test.cpp */
 
#include "download.h"

void update(unsigned long total, unsigned long part);

//This function displays  the download progress as a 

percentage. 
void showProgress(unsigned long total, unsigned long part){
  static int ii = 1;

	int val = (int)((double) part/total*100);
	
	while(val/10 == ii)
	{
		cout << val << '%' << endl;
		ii++;
	}
}

int main(int argc, char* argv[]) //what the hell IS argc? 

argv[] is the path that the file_downloader is running 

from.
{
	

	Download dlInstance; //remember you cant have non 

static class members without an instance of the class.


	//Substitute the URL of the file that you want to 

download.
	char url[] = 
		"PLACE URL HERE";


	bool reload = true;
	//reload true: trunc any existing. 
	//reload false: only download the remaining 

portions of a file.

	if(argc == 2 && !strcmp(argv[1], "reload"))
		reload = true; 

	cout << "Beginning Download.\n\n";

	try{
		if(dlInstance.download(url, reload, 

showProgress))
			cout << "\nDownload Complete!\n";
	}catch(DLExc){
		cout << "\nDownload Interrupted\n";
	}

	return 0;
}
///////////////////////////////////////////////////////////////////////
/*  dl.cpp  */
#include"download.h"
#pragma warning(disable:4996) //I'll fix the strcpy_s 

things later


/*
Download a file.

Pass the URL of the file to url. 

To reload a file, pass true to reload.

To specify an update function that is called after 
each buffer is read, pass a pointer to  that 
function as the third parameter. If no update 
function is desited, then let the third parameter
default to null.
*/

bool Download::download(char* url, bool reload,
  					void 

(*update)(unsigned long, unsigned long))
{

	ofstream fout;			//output stream
	unsigned char buf[BUF_SIZE];  //input buffer
	unsigned long numrcved =0;		  //number 

of bytes read
	unsigned long filelen =1;		  //length 

of file on disk 
	HINTERNET hIurl, hInet;		  //internet 

handles
	unsigned long contentlen =0;		//length of 

content
	unsigned long len =0;				

//length of contentlen
	unsigned long total = 0;		//running 

total of bytes received 
	char header[80];				

//holds Range header
	char files[MAX_NO_URLS];		//urls of 

the files to be downloaded.
	
	urlNotAssigned = true; //true if a throw is 

triggered before the handles are opened.
	

	try{

			if(!ishttp(url)){
				urlNotAssigned = true;
				throw DLExc("Must be HTTP 

url.");
				}

		//See if an internet connection is 

available
			if(InternetAttemptConnect(0) != 

ERROR_SUCCESS){
				urlNotAssigned = true;
				throw DLExc("ERROR 4067: 

Cannot connect to the internet.");
			}

		//Open Internet connection.
			hInet = InternetOpen("downloader",
						

INTERNET_OPEN_TYPE_DIRECT, //This is actually a pretty 

unsafe DWORD to use. I should use a proxy.
						NULL, NULL, 

0);

			if(hInet == NULL)
				throw DLExc("Can't open 

connection");

			else
				cout << "\nConnected to the 

internet.\n";


		//*********************
			
			cout << "\nWill now try to download 

the first file of the dir.\n";

			if(dlnext(url,files))
			{
				cout << "Okay, What I got 

here is ";
			for(int ii =0; ii< (sizeof(files)- 

1); ii++)
				cout << files[ii];
				cout << endl;
				cout << "\nSuccess\n";
			}
			else
				cout << "\nFailure\n";

			system("pause");




			/*Open the file specified by the 

url.
			The open stream will be returned in 

fout. 
			If reload is true then any 

preexisting file will be truncated.
			The length of any preexisting file 

(after 
			possible truncation) is returned.*/

		//filelen = openfile(files, reload, fout, 

urlNotAssigned);
		
	/*filelen =0;

		cout << "\nInput stream opened correctly.

\n";
		system("pause");

		

			//Construct header requesting range 

of data
			sprintf(header,"Range:bytes=%d-", 

filelen);

			//Open the url and request range

			//hIurl = InternetOpenUrl(hInet, 

url,
			//	header, -1,
			//	

INTERNET_FLAG_NO_CACHE_WRITE, 0);

			//if(hIurl == NULL) throw DLExc

("ERROR 8706: URL is not valid.");

			//Confirm that HTTP/1.1 or greater 

is supported.
			//if(!httpverOK(hIurl))
			 //	throw DLExc("HTTP/1.1 not 

supported");

			//Get content length
			len = sizeof contentlen;


			if(!HttpQueryInfo(hIurl,
								

HTTP_QUERY_CONTENT_LENGTH |
								

HTTP_QUERY_FLAG_NUMBER,
								

&contentlen, &len, NULL))
					throw DLExc("File 

or content length not found");

			//If existing file (if any) is not 

complete,
			//then finish downloading.
			if(filelen != contentlen && 

contentlen) //and and contentlen has value, I guess?
				do{

					//Read a buffer of 

info
					if(!

InternetReadFile(hIurl, &buf,
								

		BUF_SIZE, &numrcved))
						throw 

DLExc("Error occured during download.");

					//Write the buffer 

to disk
					fout.write((const 

char *) buf, numrcved);
					if(!fout.good())
						throw 

DLExc("Error writing file");

					total += numrcved; 

//Update running total

					//Call update 

function, if specified
					if(update && 

numrcved > 0)
						update

(contentlen+filelen, total+filelen);

				}while(numrcved > 0);
			else
				if(update)
					update(filelen, 

filelen);


	 */
	}catch(DLExc){
		fout.close();

		if(urlNotAssigned)
			cout << "\nERROR 5634: URL not 

assigned.\n";

		else{
			InternetCloseHandle(hIurl);		
			InternetCloseHandle(hInet);
		}


		throw;  //rethrow the exception for use by 

any caller
	}

	fout.close();
	//InternetCloseHandle(hIurl);
	InternetCloseHandle(hInet);

	return true;
}
	

	//Return true if HTTP version of 1.1 or greater
bool Download::httpverOK(HINTERNET hIurl){
	char str[80];
	unsigned long len = 79;

	//Get HTTP version
	if(!HttpQueryInfo(hIurl,HTTP_QUERY_VERSION,&str, 

&len, NULL))
		return false;

	//first, check major version number
	char* p = strchr(str, '/');
	p++;
	if(*p == '0') return false; //can't use HTTP 0.x

	//Now find start of minor HTTP version number
	p = strchr(str, '.');
	p++;

	//Convert to int.
	int  minorVerNum = atoi(p);

	if(minorVerNum > 0) return true; 
	
	return false;
}

//Extract the filename from the URL. Return false if 
//the filename cannot be found. 
bool Download::getfname(char *url, char *fname){
	//Find last /
	char* p = strrchr(url, '/');

	//Copy filename after the last /.
	if(p && (strlen(p) < MAX_FILENAME_SIZE)){
		p++;
		strcpy(fname,p);
		return true;
	}
	else
		return false;
}

//Open output file, intitialize the output stream
// and return the file's length. If reload is true, 
//first truncate any preexisting file.

unsigned long Download::openfile(char *url,
								

 bool reload, 
								

 ofstream &fout, bool& urlNotAssigned){
	char fname[MAX_FILENAME_SIZE];

	if(!getfname(url,fname))
		throw DLExc("File name error");

	if(!reload)
		fout.open(fname, ios::binary | ios::out | 
						ios::app | 

ios::ate);

	else
		fout.open(fname, ios::binary | ios::out | 
						

ios::trunc);

	if(!fout){
		urlNotAssigned = true;
		throw DLExc("Can't open output file.");
	}

	//get current file length
	return fout.tellp();
}

//Confirm that the URL specifies HTTP.
bool Download::ishttp(char *url){
	char str[5] = "";

	//get first four characters from url. 
	strncpy(str,url,4);

	//convert to lowercase
	for(char* p=str; *p; p++) *p = tolower(*p);

	//if(!strcmp("http", str))
		return true;

	// else append http to the beginning of the url.
		

}


bool Download::dlnext(char* url, char files[MAX_NO_URLS]){
	
	char urlFull[MAX_FILENAME_SIZE]= "";
	
	strcpy(urlFull,url);

	WIN32_FIND_DATA FindFileData; //struct for file 

information, when found by FindFirstFile
    HANDLE hFind;

	cout << "\nTarget file is *, searching for any file 

name" << endl;
	
	hFind = FindFirstFile(urlFull, &FindFileData); 

//You pass the address of the struct

	   if (hFind == INVALID_HANDLE_VALUE) 
	   {
		  cout << "FindFirstFile failed \n";
		  GetLastError();
		  return false;
	   } 
	   else 
	   {
		 cout << "The first file found is " << 

FindFileData.cFileName << "\n";

		  FindClose(hFind);
		  strcpy(files,FindFileData.cFileName);
		  return true;
	   }

   
	

		//find first file: any search which ends in 

a backslash always fails. Dont search for NULL strings

		//may need to sort results found by 

FindFirstFile(); 
		//it may not find the first file.


}



/////////////////////////////////////////////////////////////////
/*  download.h */

#include<iostream>
#include<string>
#include<windows.h>
#include<wininet.h>
#include<tchar.h>
#include<fstream>
#include<stdio.h>
#include<cstdio>
#pragma warning(disable:4996) //I'll fix the strcpy_s things later

using namespace std; 

const int MAX_ERRMSG_SIZE = 80;
const int MAX_FILENAME_SIZE = 512;
const int MAX_NO_URLS = 512;
const int BUF_SIZE = 1024;

//exception class for download errors
class DLExc {
  char err[MAX_ERRMSG_SIZE];
public:
	DLExc(char *exc) {

		if(strlen(exc) < MAX_ERRMSG_SIZE)
			strcpy(err,exc);


		//prints the error recieved.
		for(unsigned int ii =0; ii< strlen(err); ii++) //unsigned because I hate warning messages
		{
			cout << err[ii];
		}
		cout << endl;

		}

	//return a pointer to the error message
	const char* geterr(){
		return err;
	}
};


//a class for downloading files from the internet
class Download{
private:
	static bool ishttp (char* url);
	static bool httpverOK(HINTERNET hIurl);
	static bool getfname(char* url, char* fname);
	static unsigned long openfile(char* url, bool reload, ofstream& fout, bool& urlNotAssigned); //I wonder why my refVar is working when openfile() is static.. 
	bool urlNotAssigned; //To prevent the program from crashing when an incorrect url is entered 

public:
	bool download(char* url, bool restart = false, void (*update)(unsigned long, unsigned long)=NULL);
	bool dlnext(char* url, char files[MAX_NO_URLS]);
};

