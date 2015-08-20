#include <cstdio>
#include <cstdlib>
#include <zlib.h>
#include <cassert>
#include <cstring>

#define MAXLEN 16384

char *buf = new char[MAXLEN];

#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  ifdef UNDER_CE
#    include <stdlib.h>
#  endif
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif

const char *newline="\n";

int main(int argc,char**argv){
  int counter =0;
  SET_BINARY_MODE(stdout);
  gzFile file = gzdopen(fileno(stdout), "wb6");
  while(fgets(buf,MAXLEN,stdin)){
    //fprintf(stderr,"buf:%s",buf);
    if(counter==0){
      assert(buf[0]=='@');
      gzwrite(file,buf,strlen(buf));
    }else if(counter==1){
      if(strlen(buf)>101){
	gzwrite(file,buf,100);
	gzwrite(file,newline,1);
      }else
	gzwrite(file,buf,strlen(buf));
    }else if(counter==2){
      assert(buf[0]=='+');
      gzwrite(file,buf,strlen(buf));
    }else if(counter==3){
      if(strlen(buf)>101){
	gzwrite(file,buf,100);
	gzwrite(file,newline,1);
      }else
	gzwrite(file,buf,strlen(buf));
    }
    counter++;
    if(counter==4){
      counter = 0;
      //      break;
    }
  }
  
  if(counter!=0)
    fprintf(stderr,"\t-> fastqfile looks truncated\n");
  if (gzclose(file) != Z_OK) 
    fprintf(stderr,"failed gzclose");
  return 0;
}
