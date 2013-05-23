#include<iostream>
#include<stdlib.h>
#include<string>
#include<vector>
#include<list>
#include<fstream>
#include<map>
#include<algorithm>
#include<iterator>
#define MAX 100

using namespace std;
int plc=0,plc_disp=0;
class mot_mem{
	string opcode;
	int length;
	string mach_code;
public:
	mot_mem(string a,int b,string c){
		opcode=a;
		length=b;
		mach_code=c;
					}
	string get_code(int flag){
		if(flag==0) return opcode;
		else return mach_code;
				}
	int get_len(){return length;}
	void print_mot(){cout<<opcode<<length<<mach_code<<endl; }
	static mot_mem search_mot(string opc);
	static void get_mot();

	     };

class symbol_tab{
	string symbol;
	int location;
public:
	symbol_tab(string s,int loc){
		symbol=s;
		location=loc;	}
	string get_sym(){
		return symbol;}
	int get_loc(){
		return location;}

	static symbol_tab search_sym(string symbol);
	static void insert_sym(string symbol);
};

list<mot_mem>mot[MAX];
list<symbol_tab>sym[MAX];
map<string,string>reg;

int hash(string s,int div){
	int length = s.length();int k=0;
	for(int i=0;i<length;i++)
		k+=s[i]-'\0';
	return k%div;
			}


mot_mem mot_mem::search_mot(string opc){
int key=hash(opc,MAX);
list<mot_mem>::iterator i;
for(i=mot[key].begin();i!=mot[key].end();i++)
	if((*i).get_code(0)==opc) 
	return *i;
cout<<"error "<<opc<<" not in mot"<<endl;
exit(1);

}

symbol_tab symbol_tab::search_sym(string symbol){

int key=hash(symbol,MAX);
list<symbol_tab>::iterator i;
for(i=sym[key].begin();i!=sym[key].end();++i)
	if((*i).get_sym()==symbol)
	return (*i);
cout<<"error"<<sym<<"not found";
exit(1);
}

void symbol_tab::insert_sym(string symbol){
	int key=hash(symbol,MAX);
	symbol_tab temp=symbol_tab(symbol,plc);
	sym[key].push_back(temp);
}

void mot_mem::get_mot(){
	ifstream fin;	fin.open("MOT.txt");
	string opc_temp;int len,key; string mach_code;
	while(!fin.eof()){
		fin>>opc_temp;fin>>len;fin>>mach_code;
		mot_mem	temp=mot_mem(opc_temp,len,mach_code);
		key=hash(opc_temp,MAX);
 		mot[key].push_back(temp);}
	fin.close();
}

void parse_line(string line){
	string label,opcode,mcode;
	unsigned pos;
	pos=line.find(";");
	if(pos!=-1){  
		line=line.substr(0,pos);}
	
	if(line!=""){
		pos=line.find(":");
		if(pos!=-1){
			label=line.substr(0,pos);
			line=line.substr(pos+1);
			symbol_tab::insert_sym(label);}
		pos=line.find(" ");
		if(pos==-1){
			pos=line.length();}
		opcode=line.substr(0,pos);
		mot_mem temp=mot_mem::search_mot(opcode);
		plc+=temp.get_len();
			}
}



void build_map()
{
    reg["a"]="111";
    reg["b"]="000";
    reg["c"]="001";
    reg["d"]="010";
    reg["e"]="011";
    reg["h"]="100";
    reg["l"]="101";
    reg["bc"]="00";
    reg["de"]="01";
    reg["hl"]="10";
    reg["sp"]="11";
}

string replace(string str,string op1,string op2,size_t n){
	int pos=str.find('*'); string temp;
	//size_t n = std::count(str.begin(),str.end(),'*');
	if(n==6){
		temp=reg[op1]+reg[op2];
		str.replace(pos,6,temp);
		return str;}
	else if(n==3){
		temp=reg[op1];str.replace(pos,3,temp);return str+" "+op2;}
	else
	    {
	        string pair,s;char nxt;
	        nxt=op1[0]+1;
	        pair=op1+nxt;
	        s=reg[pair];
	        str.replace(pos,2,s);
	        return str+" "+op2;
	    }
	}
string to_hex(int val)//converts the integer to hex equivalengtht
{
    if(val==0)
      return "0000";
    string h="";
    char c;
    int v;
     
    while(val!=0)
    {
      v=val%16;
      c=(v>9)?('a'+v-10):('0'+v);
      h=c+h;
      val/=16;
    }
  int length=h.length(),l;
  string zeroes="";
  if(length<4)
    {
      l=4-length;
      
      while(l)
       {
          zeroes+="0";
          l--;
       }
    }
  return zeroes+h;
}

string getmach(string op,string mach){
	size_t n = std::count(mach.begin(),mach.end(),'*');int arg;
	string op1,op2;
	if(n==0 && op=="")
		return mach;
	if(n==0)
		//symbol_tab temp=symbol_tab::search_sym(op);
		return mach+" "+to_hex(symbol_tab::search_sym(op).get_loc());
        if(n==6){
		int pos=op.find(",");
		if(pos==-1){cout<<"format error"<<op<<endl;exit(1);}
		op1=op.substr(0,pos);
		op2=op.substr(pos+1);		
		return replace(mach,op1,op2,n);}
	else if (n==3 || n==2){
		int pos=op.find(",");
		op1=op;op2="";
		if(pos!=-1){
			op1=op.substr(0,pos);
			op2=op.substr(pos+1);arg=atoi(op2.c_str());op2=to_hex(arg);	}
					
		return replace(mach,op1,op2,n);				
			}
	else    {cout<<"format error"<<op<<endl;exit(1);}
				
}
void generate(string line)
{ 
 ofstream f;string mach_code;
 //f.open("output.txt");
 //if(is_org(input,1)) return;
 string label,opc,mcode;
    
 int pos;
 pos=line.find(";");
 if(pos!=-1)
 {
  line=line.substr(0,pos);
 }
 if(line!="")
  { 
   pos=line.find(':');
    if(pos!=-1)
      {
        label=line.substr(0,pos);
        line=line.substr(pos+1);
      }
    if(line!="")
     {
       pos=line.find(" ");
       if(pos==-1){
          pos=line.length();}
          opc=line.substr(0,pos); 
       
           //f<<line<<" ";
	   //cout<<"in generate"<<opc<<endl;   
           mot_mem temp=temp.search_mot(opc);
           //plc_disp+=temp.get_len();
           if(pos<line.length())
             line=line.substr(pos+1);
           else
             line="";

           mach_code=getmach(line,temp.get_code(1));
	   cout<<to_hex(plc_disp)<<" "<<mach_code<<endl;
           //f<<mach_code<<"\n";
		plc_disp+=temp.get_len();
        }
        
     }
 f.close();  
}

bool is_org(string line, int flag){
if(flag==0){
int pos=line.find("ORG");
if(pos!=-1){
	//cout<<"saw org"<<endl;
 	int sp=line.find(" ");
 	if(sp==-1){cout<<"wrong format"<<endl;exit(1);}
 	else{	string val=line.substr(sp+1); int pos_cap=val.find("H"); 
		if(pos_cap==-1)
			{ cout<<"format error hex error"<<endl;exit(1);}
	       	val=val.substr(0,pos_cap);
		//cout<<val<<endl;
		//cout<<atoi(val.c_str())<<endl;
		plc=atoi(val.c_str());//cout<<plc<<endl;
		return true; }}
 else return false;
	  }
else{int pos=line.find("ORG");
if(pos!=-1){
	//cout<<"saw org"<<endl;
 	int sp=line.find(" ");
 	if(sp==-1){cout<<"wrong format"<<endl;exit(1);}
 	else{	string val=line.substr(sp+1); int pos_cap=val.find("H"); 
		if(pos_cap==-1)
			{ cout<<"format error hex error"<<endl;exit(1);}
	       	val=val.substr(0,pos_cap);
		//cout<<val<<endl;
		//cout<<atoi(val.c_str())<<endl;
		plc_disp=atoi(val.c_str());
		//cout<<plc_disp<<endl;
		return true; }}
 else return false;	 
	}
}



void load_file(){
 ifstream fin;
 fin.open("temp.txt");
 string input;
 while(getline(fin,input)){
	if(!is_org(input,0))
	parse_line(input);}
	
 fin.close();
}

void output(){
	ifstream fin;
	fin.open("temp.txt");
 string input;
 while(getline(fin,input)){
	if(!(is_org(input,1)))
	//if((int pos=input.find("ORG"))==-1)
	generate(input);}
 fin.close();           
}


main(){
build_map();
mot_mem::get_mot();
load_file();
output();
cout<<"Symbol Table:"<<endl;
list<symbol_tab>::iterator it;
cout<<"Symbol Name"<<" "<<"Symbol Address"<<endl;
for(int i=0; i<MAX; i++){
for(it=sym[i].begin(); it!=sym[i].end(); it++)
 cout<<(*it).get_sym()<<"        "<<to_hex((*it).get_loc())<<endl;}
}



