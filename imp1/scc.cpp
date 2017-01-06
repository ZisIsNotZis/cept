#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <cstring>
#include <unistd.h>
using namespace std;

enum{DefNoType,FuncdefNoName,FuncdefNoPair};

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
		stat=isList;
		while(r<s.size()){
			for(;s[r]==' '||s[r]=='\t';++r);
			parse tmp;
			parseBack:
			l=r;
			switch(s[r]){
			case '[':
				tmp=parse(s.substr(l+1,SIZE_MAX),&r);
				r+=l+2;
				break;
			case ']':
				if(retPos)*retPos=r;
				return;
			case ';':
				if(list.size()<1)throw DefNoType;
				stat=isType;
				++r;
				goto parseBack;
			case '\'':
				if(s[++r]=='\'')			
					stat=isStr;
				else{
					if(s[r]=='\\')++r;
					if(s[++r]!='\'')stat=isStr;
				}
				r=l;
				if(stat==isStr)s[r]='"';
				//fall through
			default:
				if(s[r]=='e'&&s[r+1]=='n'&&s[r+2]=='u'&&s[r+3]=='m')
				for(;s[r]!=' '&&s[r]!='\t'&&s[r]!=';'&&s[r]!=']'&&r<s.size();++r)if(s[r]=='\\')++r;
				tmp.atom=s.substr(l,r-l);			
				tmp.stat=isAtom;
			}
			if(stat==isStr){
				tmp.atom[tmp.atom.size()-1]='"';
				stat=isList;
			}
			if(stat==isType){
				if(tmp.stat==isList){
					vector<parse> &typeList=list[list.size()-1].list;
					vector<parse> &tmpList=tmp.list;
					if(typeList.size()!=tmpList.size())throw FuncdefNoPair;
					for(size_t i=0;i<typeList.size();++i){
						typeList[i].atom=typeList[i].explaiN()+' '+tmpList[i].explaiN();
						typeList[i].stat=isAtom;
					}
				}else{
					def.emplace_back(list[list.size()-1],tmp.explaiN());
					list[list.size()-1]=tmp;
					stat=isList;
				}
				stat=isList;
			}else
				list.push_back(tmp);
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
		//def.resize(0)
		//TODO: why don't this work
		while(def.size()>0)
			def.pop_back();
		s<<explaiN();
		return s.str();
	}
	string explaiN(){
		if(stat){
			if(list.size()<1)return "";
			if(list.size()<2&&list[0].stat==isList)return list[0].explaiN();
			ostringstream s;
			s<<list[0].explaiN()<<'(';
			if(list.size()>1){
				s<<list[1].explaiN();
				for(size_t i=2;i<list.size();++i)s<<','<<list[i].explaiN();	
			}
			s<<')';
			return s.str();
		}else return atom;
	}
	enum{isAtom=0,isList,isStr,isType} stat;
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
		if(j==0||argv[i][++j]=='c'||argv[i][j]=='C'){
			cerr<<argv[0]<<": "<<argv[i]<<": Illegal file extension\n";
			continue;
		}
		argv[i][j]='c';
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
				f<<parse(s).explain()<<'\n';
			}catch(int n){
				switch(n){
				case DefNoType:cerr<<argv[0]<<": "<<argv[i]<<' '<<line<<": Expected type before ';'\n";continue;
				case FuncdefNoName:cerr<<argv[0]<<": "<<argv[i]<<' '<<line<<": Expected function name before '='\n";continue;
				case FuncdefNoPair:cerr<<argv[0]<<": "<<argv[i]<<' '<<line<<": Expected atom number aside ';' to be the same\n";continue;
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
