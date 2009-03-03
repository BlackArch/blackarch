/* V1.0 Security Patches and upgrades by Mike Schiraldi */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>

char *type = NULL;
char *description = NULL;
char *google_server = NULL;
char *proxyaddr = NULL;
char recvbuf[65000];
char pretty_query[1024];
char mid_query[1024];
char out_query[1024];
char *site = NULL;
char total_string[256];
char *xtra_stuff = NULL;

int aflag = 0;
int dflag = 0;
int oflag = 0;
int pflag = 0;
int sflag = 0;
int vflag = 0;
int xflag = 0;
int total = 0;
int cooked = 0;
int proxyport = 0;
long results = 0;

struct hostent *he;
struct hostent *pr;

long google_query(char *gooflag, char *type, char *value );
void writeit(FILE *fp, char *out_query, char *mid_query, int results, char *flag);
long parse_results(char *recvbuf);
char* inet_convert(char *inbound, int dflag);
int inet_send(char *sendbuf); 
int usage();

int main(int argc, char *argv[])
{

  extern char *description;
  extern char *proxyaddr;
  extern char *google_server;
  extern char pretty_query[1024];
  extern char mid_query[1024];
  extern char out_query[1024];
  extern char *site;
  extern char *xtra_stuff;
  extern char total_string[256];
  extern int dflag,oflag,sflag,vflag,xflag;
  extern int total;
  extern int cooked;
  extern int proxyport;
  extern long results;
  
  char *in_query = NULL;
  char list[1024];
  char *proxy = NULL;
  char *in_filename = NULL;
  char *out_filename = NULL;
  char *type,*value,*build_buf;
  
  int c;
  int fflag = 0, qflag = 0;
  int indexofflag = 0;
  int in_count=0;
  
  FILE *scanlist;
  FILE *outfile;

  time_t timer;
  
  build_buf=calloc(256, sizeof(char));
  
  if (argc == 1) usage();
  
  while ((c = getopt(argc,argv,"acdhvi:o:p:q:s:t:x:")) != -1) {
     switch (c) {
     case 'd': //decode flag
	dflag=1;
	break;
     case 'p': // proxy server
  	  if ( (proxyaddr=calloc(256, sizeof(char))) == NULL)
  	  {
		  printf("Error allocating memory for proxyaddr. Aborting.\n");
		  exit(1);
  	  }
	  if (strstr(optarg,":") == NULL) {
		printf("Error: Proxy address must be like address:port.\n");
		usage();
	  }
	  pflag=1;
	  proxy=optarg;
          snprintf(proxyaddr,256,"%s",strtok(proxy,":"));
          proxyport=atoi(strtok(NULL,"\0"));
          //printf("%s|%d",proxyaddr,proxyport);
	  break;
     case 'q': // inline query
	  qflag=1;
	  in_query=optarg;
	  break;
     case 'i': // input query file
	  fflag=1;
	  in_filename=optarg;
	  break;
     case 'o': // output html file
	  oflag=1;
	  out_filename=optarg;
	  break;
     case 's': // site variable
	  sflag=1;
	  site=optarg;
	  break;
     case 't': // target_server
  	  if ( (google_server=calloc(256, sizeof(char))) == NULL)
  	  {
		  printf("Error allocating memory for google_server. Aborting.\n");
		  exit(1);
  	  }
	  google_server=optarg;
	  break;
     case 'v': // verbose
	  vflag=1;
	  break;
     case 'x': // extra stuff for appliances
	  xflag=1;
	  xtra_stuff=optarg;
	  break;
     case 'h':  //help
	  usage();
     default: 
	  usage();
     }
  }

  //clean up the server, if necessary
  if (strstr(google_server,"http://") != NULL)
	strncpy(google_server,google_server+7,256);

  // A bunch of errors for mixed-up user parameters
  if (strlen(google_server) == 0) {
	printf("You must provide the google server with the -t argument!\n");
	usage();
  }

  if (strstr(google_server,"www.google.com") != NULL) {
    printf("\n***!!! WARNING: You are querying a www.google.com server !!!***\n");
    printf("This tool was designed to query Google appliances, not the google.com website.\n");
    printf("The google.com scanning functionality is included for EDUCATIONAL PURPOSES ONLY \n");
    printf("to help webmasters determine the potential Google exposure of their sites.\n");
    printf("\nDo you acknowledge that: \n");
    printf("  - You are knowingly violating Google's terms of service found at\n");
    printf("    http://www.google.com/terms_of_service.html\n");
    printf("  - You are using this tool to assess your own web site's exposure\n");
    printf("  - The use of this tool in this way is not condoned by the author\n");
    printf("  - You will not hold the author liable in any way for the use of this tool\n\n");
    printf("Agree? (y/n) [n] ");
    if ( getchar() != 'y' ) exit(1);
  }

  if (!qflag && !fflag) {
     printf("Error: supply either -q or -f flag.\n");
     usage();
  } 
  if (qflag && fflag) {
     printf("Error: -q and -f should not be used together.\n");
     printf("-q allows for a single Google query from the command line,\n");
     printf("-f allows for multiple Google queries read from a file.\n");
     usage();
  }

  // OK, the user wants output. Open the file, write the header.
  if (oflag) {
    if (( outfile = fopen(out_filename,"a")) == NULL) {
       perror(out_filename);
       exit(1);
    }
    writeit(outfile,"","<br><CENTER><b>Gooscan Results</b></CENTER><br>",0,"raw");
    snprintf(build_buf,256,"<center>site: %s<br>input file: %s<br>",\
		sflag?site:"none" , fflag?in_filename:"none");
    writeit(outfile," ",build_buf,0,"raw");
    timer=time(NULL);
    snprintf(build_buf,256,"Executed: %s</CENTER><br>",asctime(localtime(&timer)));
    writeit(outfile," ",build_buf,0,"raw");
    writeit(outfile,NULL,NULL,0,"header");
  }
  
  // OK, user wants one query. Run it, write it, close the outfile, exit.
  if (qflag) {
    results=google_query("raw","raw",in_query);
    printf("\"%s\" returned %ld results.\n",in_query,results);
    if (oflag) writeit(outfile,out_query,pretty_query,results,"line");
  } 

  // Here's the fun stuff. Queries from an infile.
  else if (fflag) {

     if (sflag) printf("Searching google for site %s...\n",site);

      //fopen the infile
      if (fflag) 
	  if (( scanlist = fopen(in_filename,"r")) == NULL) {
		  perror(in_filename);
		  exit(1);
	  }
 

	// If we're searching a site, do a generic index.of. 
	// If that fails, never try index.of search again.
   	if (sflag) {
          results=(google_query("raw","raw","intitle:index.of"));
	  if ( results == 0) {
            indexofflag = 0;
	    printf("Generic index.of search returned no results.\n");
	    printf("Skipping index.of tests.\n");
	  }
	  else {
	    indexofflag = 1; 
	    printf("Generic index.of search returned %ld results\n",results);
	  }
	}
	else indexofflag = 1;

	//The main file reading loop
	while ( fgets(list,1024,scanlist) != NULL) {
		total =0;
		cooked=0;
		memset(total_string,'\0',256);
		memset(mid_query,'\0',256);
		memset(out_query,'\0',256);
   	
		if ((type=calloc(48, sizeof(char))) == NULL)
		{
		  printf("Error allocating memory for type. Aborting.\n");
		  exit(1);
		}
		if ((value=calloc(200, sizeof(char))) == NULL)
		{
		  printf("Error allocating memory for value. Aborting.\n");
		  exit(1);
		}
		if ((description=calloc(2048, sizeof(char))) == NULL)
		{
		  printf("Error allocating memory for description. Aborting.\n");
		  exit(1);
		}

		snprintf(type,48,"%s",strtok(list,"|"));
		snprintf(value,200,"%s",strtok(NULL,"|"));
		in_count=atoi(strtok(NULL,"|"));
		//printf("type:%s,value:%s:count:%d,description:%s\n",type,value,in_count,description);
		if ( type != NULL && value != NULL) {
		   if ( strcmp(type,"cooked") == 0) cooked=1;
		   // for "file" lines, we do inurl and indexof searches
		   if ( strcmp(type,"file") == 0 )  {
		      if (indexofflag == 1) {
			results=google_query("indexof",type,value);	
  			if (oflag) writeit(outfile,out_query,pretty_query,results,"line");
		      }
		      //inurl: test
		      results=google_query("inurl",type,value);
  	     	      if (oflag) writeit(outfile,out_query,pretty_query,results,"line");

		      if (results >0 || vflag) printf("%sTotal:%d\n",total_string,total);
		   }
		   else {
			results=google_query(type,"Results",value);
  	     	        if (results>=0) {
			   if (oflag) writeit(outfile,out_query,pretty_query,results,"line");
		        if (results >0 || vflag) printf("%s\n",total_string);
			}
			else if (vflag) printf("Unknown type in data file (%s). Skipping...\n",type);
		   }
		   free(description);
		   free(type);
		   free(value);
 		   //printf("mid_q:%s\n",mid_query);
		}
	}
	fclose(scanlist);
  }
	
  if (oflag) {
    writeit(outfile,NULL,NULL,0,"footer"); 
    fclose(outfile);
  }

//printf("out_query: %s\n",out_query);
  return 0;
}

long google_query(char *gooflag, char *type, char *value ) {
  extern char pretty_query[1024]; //a human-readable version
  extern char mid_query[1024];//a version for the web server
  extern char *site;
  extern char *xtra_stuff;
  extern char total_string[256];
  extern int sflag;
  extern int vflag;
  extern long results;

  char scratch[1024];

  if (strlen(total_string) ==0) {
	//printf("null total string");
        sprintf(total_string,"%s:%s",type,value);
  }


  if (vflag) printf("(verbose) command line query => %s\n",value);

  if (strcmp(gooflag,"indexof") ==0 ) {
     sprintf(pretty_query, "intitle:index.of+%s",value); 
  }
  else if (strcmp(gooflag,"inurl") ==0 ) {
     sprintf(pretty_query, "inurl:%s",value); 
  }
  else if (strcmp(gooflag,"intitle") ==0 ) {
     sprintf(pretty_query, "intitle:%s",value); 
  }
  else if (strcmp(gooflag,"raw") ==0 ) sprintf(pretty_query,"%s",value); 
  else if (strcmp(gooflag,"filetype") ==0) {
	sprintf(pretty_query,"filetype:%s",value);
  }
  else return(-1);

  // pretty_query will not have any of the below ugliness
  sprintf(mid_query,"&q=%s",pretty_query);
  
  if (sflag) {
	sprintf(scratch,"%s+site:%s",mid_query,site); 
	strncpy(mid_query,scratch,1024);
	if (vflag)
	   printf("(verbose) site added. query is now: %s\n",mid_query);
  }
  
  if (xflag) {
	sprintf(scratch,"%s&%s",mid_query,xtra_stuff); 
	strncpy(mid_query,scratch,1024);
	if (vflag)
	   printf("(verbose) extra stuff added. query is now: %s\n",mid_query);
  }

  if (vflag) printf("(verbose) Sending query => %s\n",mid_query);


  if (inet_send(mid_query) == 1) { 
 	printf("Error sending query.\n");
	return(0);
   }
  else {  results=parse_results(recvbuf); }
  sprintf(scratch,"%s %s:%ld ",total_string,gooflag,results);
  strncpy(total_string,scratch,256);
  total+=results;
  return(results);

}

void writeit(FILE *fp, char *out_query, char *mid_query, int results, char *flag) {
   extern char *google_server; 
   if (strcmp(flag,"header") == 0) {
	fprintf(fp,"<style>BODY {FONT-FAMILY: Verdana,Helvetica; FONT-SIZE: 12px}</style>");
	fprintf(fp,"<CENTER><TABLE BORDER=1>\n");
	fprintf(fp,"<TR><TD>Search</TD><TD>Link</TD><TD>Results</TD></TR>");
   }
   else if (strcmp(flag,"line") == 0) {
     char * bold_start = (results == 0) ? "" : "<b>";
     char * bold_end   = (results == 0) ? "" : "</b>";
     fprintf(fp,"<TR><TD>%s%s%s</TD><TD><A HREF=\"http://%s/search?%s\">link</A></TD>"
                "<TD align=\"RIGHT\">%d</TD></TR>\n", bold_start,mid_query,bold_end,
             google_server,inet_convert(out_query,1),results);
   }
   else if (strcmp(flag,"footer") == 0) {
	fprintf(fp,"</TABLE>\n");
	fprintf(fp,"<BR>gooscan by j0hnny<br><A HREF=\"http://johnny.ihackstuff.com\">http://johnny.ihackstuff.com</A><BR>");
   }
   else if (strcmp(flag,"raw") == 0)  fprintf(fp,"%s",mid_query);

   //fflush(fp);
}



//Pull apart Google's HTML to find this string: 
//   "Results <b>1</b> - <b>10</b> of <b>1000</b>."
//   (numbers and [of|of about] may vary) This means we got hits.
long parse_results(char *recvbuf) {
  extern int oflag,vflag;
  char *errorLine;
  char *ptr;
  char *buf;
  char copy[25];
  int i =0, j =0;

  errorLine=calloc(1024, sizeof(char));

  // Grab the first line from the web server
  strncat(errorLine,recvbuf,(strcspn(recvbuf,"\n")-1));

  if (vflag) 
    printf("(verbose) Status line from server: \"%s\"\n",errorLine);

  if (strstr(errorLine,"200") == NULL) {
    printf("Error! Received \"%s\" from server!\n",errorLine); 
    return(0);
  }

  ptr=strstr(recvbuf,"Results <b>");
  
  if (ptr != NULL) {
          // Find the actual number of results. 
          // Follow </b> tags, skip the "of" or "of about", then
      	  //   read between the > and < for the number. gay, i know.
          buf=strtok(ptr,"Results <b>");
          buf=strtok(NULL," </b>");
          buf=strtok(NULL," </b>");
          buf=strtok(NULL,">");
          buf=strtok(NULL,">");
          buf=strtok(NULL,"<");
	  //printf("%s returned %s results.\n",mid_query,buf);
     	  if (buf != 0) {
	     j=0;
             for (i=0;i<strlen(buf);i++) {
	       if (buf[i] != ',') {
		   copy[j]=buf[i];
		   j++;
  	       }
	     }
             copy[j++]='\0';
	     return(atol(copy));
         } else return(0);
  }
  else {
     	ptr=strstr(recvbuf,"of about <b>");
	if (ptr != NULL) {
	   buf=strtok(ptr,"of about <b>");
	   //printf("|%s|",buf);
 	   return(atol(buf));
	}
  }
  return(0);
	  

}


//
// Convert a string (the query) to modified web-hex (! becomes %25, etc)
//    oddities: (space) becomes   +
//	           +    stays     +
//		(alpha) stays  (alpha)
//
char* inet_convert(char *inbound,int dflag) {

  int i;
  extern int vflag;
  char *outbound;
  char *scratch;
  outbound=calloc(1024, sizeof(char));
  scratch=calloc(1024, sizeof(char));

  for ( i=0;i<strlen(inbound);i++){
    //if it's a space or a plus, make it a plus 
    if (inbound[i] == ' '|| inbound[i] == '+')
	  sprintf(scratch,"%c",'+');
    //if it's an alpha, keep it an alpha
    else if (inbound[i] == '&' || inbound[i] == '=')
	 sprintf(scratch,"%c",inbound[i]);
    else if ( (inbound[i] >= 65 && inbound[i] <= 90 )  ||
              (inbound[i] >= 97 && inbound[i] <= 122)     )   
              sprintf(scratch,"%c", inbound[i]);
    //else  sprintf(outbound,"%s%%%x",outbound,inbound[i]);
    else {
	// if the dflag is off, just print the character
	if (dflag == 0) sprintf(scratch,"%c",inbound[i]);
	// otherwise, print it as a hex character 
	if (dflag == 1) sprintf(scratch,"%%%x",inbound[i]);
    }
    strcat(outbound, scratch);
  }
  //strncpy(out_query, scratch, 1024);
  if (vflag) printf("(verbose) post-converted string: \"%s\"\n",outbound);
  //exit(1);
  return(outbound);
}


//
//Format and send constructed string to Google
//
int inet_send(char *sendbuf) {

  extern char *proxyaddr;
  extern char *google_server;
  extern char recvbuf[65000];
  extern char mid_query[1024];
  extern char out_query[1024];
  extern int proxyport;
  extern int cooked,vflag,pflag,dflag;
  char *get_line;
  int sock;
  struct sockaddr_in http;
  extern struct hostent *he;

  if ( (get_line=calloc(1024, sizeof(char))) == NULL)
  {
	printf("Error allocating memory for get_line. Aborting.\n");
	exit(1);
  }
  
  // if you pass in a "cooked" line, it's assumed that this
  // is ready to send without any formatting
  if(cooked) 
	strncpy(get_line,out_query,1024);
  else
    {
      strncpy(out_query,inet_convert(sendbuf,dflag),1024);
      if (!pflag) 
  	sprintf(get_line,"GET /search?%s HTTP/1.0\n\n",out_query);
      else 
  	sprintf(get_line,"GET http://%s/search?%s HTTP/1.0\n\n",google_server,out_query);
    }

  if (vflag) printf("(verbose) GET Line => %s\n",get_line);
  // if there's no resolved hostname...
  if (he == NULL) {
    printf("doing lookup of %s...\n",google_server);
    // get one.
    
    if (pflag) {
	if ((he = gethostbyname(proxyaddr)) == NULL) {
	  perror("gethostbyname");
	  exit(1);
	}
    }
    else {
	if ((he = gethostbyname(google_server)) == NULL) {
	  printf("Host not found: %s\n",google_server);
	  exit(1);
        }
    }
  }

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
 	  perror("socket");
	  exit(1);
  }
  
  http.sin_family = AF_INET;
  if (pflag) http.sin_port = htons(proxyport);
  else       http.sin_port = htons(80);
  http.sin_addr   = *((struct in_addr *) he->h_addr);
  bzero(&(http.sin_zero), 8);
  
  if (connect(sock, (struct sockaddr *) & http, sizeof(struct sockaddr)) == -1) 
  {
    close(sock);
    perror("connect");
    return(1);
  }
  
  send(sock, get_line, strlen(get_line), 0);
  recv(sock, recvbuf, sizeof(recvbuf), MSG_WAITALL);
  close(sock);
  
  return(0);
}


int usage() {
	printf("gooscan <-q query | -i query_file> <-t target>\n");
	printf("       	[-o output_file] [-p proxy:port] [-v] [-d]                     \n");
	printf("       	[-s site] [-x xtra_appliance_fields]\n");
	printf("	----------------------------------------------------------------\n");
	printf("       	(query)       is a standard google query (EX: \"intitle:index.of\")\n");
	printf("       	(query_file)  is a list of google queries (see README)\n");
	printf("	(target)      is the Google appliance/server\n");
	printf("	(output_file) is where the HTML-formatted list of results goes\n");
	printf("	(proxy:port)  address:port of a valid HTTP proxy for bouncing\n");
	printf("	(site)        restricts search to one domain, like microsoft.com\n");
	printf("        (xtra_appliance_fields) are required for appliance scans\n");
	printf(" 	-v turns on verbose mode\n");
	printf(" 	-d hex-encodes all non-alpha characters\n");
	printf("Friendly example: \n");
	printf("gooscan -t google.fda.gov -q food \n");
	printf("        -x \"&client=FDA&site=FDA&output=xml_no_dtd&oe=&lr=&proxystylesheet=FDA\"\n");
	printf("Google terms-of-service violations:\n");
	printf("gooscan -t www.google.com -q \"linux\" \n");
	printf("gooscan -t www.google.com -q \"linux\" -s microsoft.com \n");
	printf("gooscan -t www.google.com -f gdork.gs\n\n");

	printf("Gooscan google scanner by j0hnny http://johnny.ihackstuff.com\n");

	exit(1);
}
