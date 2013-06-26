// code for handling macro in the input file
#include<iostream>
#include<stdlib.h>
#include<string>
#include<list>//to use doubly linked lists
#include<fstream>//for file I/O
#include<vector>//for re-sizable array
#include<map>
#define MAX 100

using namespace std;

struct MacroTable
{
  string m_name;
	string m_defn;
};
/* hash table operations*/
list<MacroTable>mTable[MAX];

int generate_hash(int key,int sz)
{
 return key%sz;
}

int hash(string s_name,int sz)
{
 int l=s_name.length();
 int k=0;
 for(int i=0;i<l;i++)
   {
     k+=s_name[i]-'\0';//ASCII code of the character
   }
 return generate_hash(k,sz);
}


string searchfromMtable(string m_name)//return the MACRO defn
{
    int key=hash(m_name,MAX);
    list<MacroTable>::iterator it;
    for(it=mTable[key].begin();it!=mTable[key].end();it++)
    {
        if(it->m_name==m_name)
            return it->m_defn;
    }
   cout<<"Fatal error Macro:"<<m_name<<" not defined!" ;
   exit(1);
}

bool doesExist(string m_name)//returns whether the macro exists
{
    int key=hash(m_name,MAX);
    list<MacroTable>::iterator it;
    for(it=mTable[key].begin();it!=mTable[key].end();it++)
    {
        if(it->m_name==m_name)
            return true;
    }
    return false;
}

//int plc;

void insertIntoMacTable(MacroTable M)
{
	int k=hash(M.m_name,MAX);
	mTable[k].push_back(M);
}

/*Hash table operations over*/


void printMacroTable()
{
  int i;
  cout<<"\nPrinting the macro table:\n" ;
  list<MacroTable>::iterator it;
  for(i=0;i<MAX;i++)
  {
      for(it=mTable[i].begin();it!=mTable[i].end();it++)
          cout<<it->m_name<<"\n"<<it->m_defn<<"\n";
  } 
}

int findchar(string S,char c)
 {
   int i,l=S.length();
   for(i=0;i<l;i++)
     if(S[i]==c)
        return i;
   return -1;//not found

  }
string toUpperCase(string str)
{
 int i,l=str.length();
 string up="";
 for(i=0;i<l;i++)
  {
   if(str[i]>='A' && str[i]<='Z')
     up+=str[i];
   else
     up+='A'+str[i]-'a';
  }
 return up;
}
string ToString(int val)
{
 string s="";
 char c;
 int n;
 if(val==0)
   return "0";
 while(val!=0)
 {
  n=val%10;
  c='0'+n;
  s=c+s;
  val/=10;
 }
 return s;
}

bool is_Macro_defn(string in)
{
    int p=findchar(in,' ');
    string chk;
    if(p!=-1)//a space exists
    {
        chk=in.substr(p+1);
        if(toUpperCase(chk)=="MACRO")
            return true;
        
    }
  return false;
}

bool isMacro(string str,string &name)
{
 int p=findchar(str,';');
 if(p!=-1)
 {
     str=str.substr(0,p);//remove the comment
 }
 p=findchar(str,':');
 if(p!=-1)
 {
     str=str.substr(p+1);//remove the label
 }
 if(str!="")//there exists something
 {
     if(doesExist(str))
     {
         name=str;
         return true;
     }
 }
 return false;
}
void preProcessFile()
{
  ifstream fin;
  fin.open("in.txt");
  string in;
  string mcro;
  MacroTable mEntry;
  //cout<<"\nProcessing the file!";
  while(getline(fin,in))
   {
      if(is_Macro_defn(in))//if the input string is a macro definition
      {
        int pos=findchar(in,' ');
        mEntry.m_name=in.substr(0,pos);
        mcro="";
        while(getline(fin,in))
        {
            if(toUpperCase(in)=="ENDM")
            {
                
                mEntry.m_defn=mcro;
                insertIntoMacTable(mEntry);
                break;
            }
            else
            {
                mcro+=in;
                mcro+="\n";
            }
        }
      }
   }
  fin.close(); 
}
void replaceMacro()//replace all the MACRO definitions 
{
  ifstream fin;
  ofstream fout;
  fin.open("in.txt");
  fout.open("temp.txt");
  string in;
  string m_name="";
  while(getline(fin,in))
   {
      if(is_Macro_defn(in))
      {
          cout<<in<<" is a MACRO definition!";
          while(getline(fin,in))
          {
              if(toUpperCase(in)=="ENDM")
                  break;
          }
      }
      else if(isMacro(in,m_name))//if it is a macro declaration
      {
          string defn=searchfromMtable(m_name);
          fout<<defn;//replace it with the definition
          m_name="";
      }
      else
          fout<<in<<"\n";
          
   }
  fin.close();
  fout.close();   
}
main()
{
 preProcessFile();//preprocess the file to find out macro defns
 replaceMacro();//replace the macro's with their defns
 printMacroTable();
 }
