#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <cstring>
#include <unistd.h>
using namespace std;

constexpr size_t DefNoType=1;
constexpr size_t DefNoName=2;
bool afterFunc=false;

struct parse;
struct deF{
	deF(const parse& a,const string& b);
	~deF();
	parse *type;
	string var;
};
vector<deF> def;

struct parse{
	parse(){}
	parse(string s,size_t* retPos=0){
		size_t l,r=0;
		char staT=0;
		stat=1;
		while(r<s.size()){
			for(;s[r]==' '||s[r]=='\t';++r);
			l=r;
			switch(s[r]){
			case '[':list.emplace_back(s.substr(l+1,SIZE_MAX),&r);break;
			case ']':if(retPos)*retPos=r+1;break;
			case ';':
				if(list.size()<1)throw DefNoType;
				staT=2;
				++r;
				goto parseDefault;
			case '\'':
				if(s[++r]=='\'')
					staT=1;
				else{
					if(s[r]=='\\')++r;
					if(s[++r]!='\'')staT=1;
				}
				r=l;
				if(staT)s[r]='"';
			default:parseDefault:
				for(;s[r]!=' '&&s[r]!='\t'&&s[r]!=';'&&s[r]!=']'&&r<s.size();++r)if(s[r]=='\\')++r;
				switch(staT){
				case 1:s[r-1]='"';break;
				case 2:
					def.emplace_back(list[list.size()-1],s.substr(l,r-l));
					list.resize(list.size()-1);
					continue;
				}
				parse tmp;
				tmp.atom=s.substr(l,r-l);
				tmp.stat=0;
				list.push_back(tmp);
			}
		}
	}
	string explain(){
		ostringstream s;
		for(size_t i=0;i<def.size();++i){
			string S=def[i].type->explaiN();
			size_t j;
			for(j=1;j<S.size()&&S[j]!='(';++j);
			if(S[j]=='(')s<<S.substr(0,j)<<' '<<def[i].var<<S.substr(j,SIZE_MAX);
			else s<<S<<' '<<def[i].var;
			s<<';';
		}
		s<<explaiN();
		return s.str();
	}
	string explaiN(){
		if(stat){
			if(list.size()<1)return "";
			ostringstream s;
			if(afterFunc){
				afterFunc=false;
				for(size_t i=0;i<list.size();++i)s<<"auto&"<<list[i].explaiN()<<"=mylang"<<i<<';';
			}else if(list[list.size()-1].explaiN()=="="){
				if(list.size()<2)throw DefNoName;
				s<<"auto "<<list[0].explaiN()<<'(';
				if(list.size()>2){
					s<<"auto "<<list[1].explaiN();
					for(size_t i=2;i<list.size()-1;++i)s<<",auto "<<list[i].explaiN();
					s<<')';
				}
				afterFunc=true;
			}else{
				s<<list[0].explaiN()<<'(';
				if(list.size()>1){
					for(size_t i=1;i<list.size()-1;++i)s<<list[i].explaiN()<<',';	
					s<<list[list.size()-1].explaiN();
				}
				s<<')';	
			}
			return s.str();
		}else return atom;
	}
	char stat;
	vector<parse> list;
	string atom;
};

deF::deF(const parse& a,const string& b):var(b){
	type=new parse;
	*type=a;
}
deF::~deF(){
	delete type;
}

int main(int argc,char** argv){
{	int i;
	size_t j,J=0,line;
	ifstream F;
	ofstream f;
	string s;
	for(i=argc-1;i>0;--i){
		if(argv[i][0]=='-')continue;
		F.open(argv[i]);
		if(!F){
			cerr<<argv[0]<<": "<<argv[i]<<": Cannot open such file\n"
			    <<argv[0]<<": note: Option argument must be attached to option, like -oOutput\n";
			continue;
		}
		for(j=strlen(argv[i])-2;argv[i][j]!='.'&&j>0;--j);
		if(j==0||argv[i][++j]<'A'||argv[i][j]>'Z'){
			cerr<<argv[0]<<": "<<argv[i]<<": Illegal file extension\n";
			continue;
		}
		argv[i][j]=tolower(argv[i][j]);
		f.open(argv[i]);
		if(!f){
			cerr<<argv[0]<<": "<<argv[i]<<": Cannot open output file\n";
			continue;
		}
		f<<"#include\"scc.h\"\n";
		line=0;
		while(getline(F,s)){
			++line;
			for(j=0;s[j]=='\t';++j);
			if(j>J)
				for(;j>J;++J)f<<'{';
			else{
				f<<';';
				for(;j<J;--J)f<<'}';
			}
			try{
				f<<parse(s).explain();
			}catch(int n){
				switch(n){
				case DefNoType:cerr<<argv[0]<<": "<<argv[i]<<' '<<line<<": Expect type before var\n";continue;
				case DefNoName:cerr<<argv[0]<<": "<<argv[i]<<' '<<line<<": Expect name after type\n";continue;
				}
			}

		}
		for(;J>0;--J)f<<'}';
	}
}	argv[0]=(char*)"g++";
	execvp(argv[0],argv);
	argv[0]=(char*)"clang++";
	execvp(argv[0],argv);
	argv[0]=(char*)"icl";
	execvp(argv[0],argv);
	cerr<<argv[0]<<": Files converted, but cannot open either gcc, clang, or icc\n";
}		
