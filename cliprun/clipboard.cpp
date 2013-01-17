#include "utils.h"
#include "vnclib.h"

using namespace std;

std::string
trimword(const std::string& str){
  std::string s;
  int i;
  for(i=0;i<str.length();i++){
    if(str[i]!=' '&&
       str[i]!='('&&
       str[i]!=':'&&
       str[i]!='\n'&&
       str[i]!='\r'&&
       str[i]!='\t')
      break;
  }
  
  for(;i<str.length();i++){
    if(str[i]==' '||
       str[i]=='('||
       str[i]==':'||
       str[i]=='\n'||
       str[i]=='\r'||
       str[i]=='\t')
      break;
    s+=str[i];
  }
    
  return s;
}

std::string
trimline(const std::string& str){
  std::string s;
  int i;
  for(i=0;i<str.length();i++){
    if(str[i]!=' '&&
       str[i]!='('&&
       str[i]!=':'&&
       str[i]!='\n'&&
       str[i]!='\r'&&
       str[i]!='\t')
      break;
  }
  
  for(;i<str.length();i++){
    if(str[i]=='\n'||
       str[i]=='\r')
      break;
    s+=str[i];
  }
    
  return s;
}

std::string
chomp(const std::string& str){
  std::string s;
  int i=0;
  for(;i<str.length();i++){
    if(str[i]=='\n'||
       str[i]=='\r')
      break;
    s+=str[i];
  }
    
  return s;
}

void
get_cuttext(VNC_Client* vncp,const string& cuttext){
  std::string str=trimword(cuttext);
  std::string w3m=strf("w3m -cols 60 -dump 'http://eow.alc.co.jp/%s/UTF-8/?ref=sa'  ",str.c_str());
  printf("\033[2J");
  printf("len:%d txt:%s\n",
	 str.length(),
	 str.c_str());
  FILE* in=popen(w3m.c_str(),"r");
  char buf[1024];
  int  i=0;
  int  s=0;
  if(in==NULL)
    return;
  
  while(fgets(buf,sizeof(buf),in)!=NULL){
    // if(strstr(buf,str.c_str())!=NULL)
    //   s=1;
    if(strstr(buf,"EDP")!=NULL){
      s++;
    }
    if(i>=1&&i<30){
      std::string s=chomp(buf);
      if(s.length()>0)
	printf("%s\n",s.c_str());
    }
    if(s>=2)
      i++;
  }
  fclose(in);
}

