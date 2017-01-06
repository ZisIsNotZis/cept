/*	checklist
	bracket			perfect
	type=type		defer, crash when two occour at same line, no idea why or how
	type=range		perfect
	type=enum		perfect with strict grammar
	type=func		perfect with strict grammar
	span			perfect with strict grammar
	charToStr		perfect
	backfix			perfect
	atom				prefect

	Not trying to implement non-strict grammar at all. Usually it's impossible
	since we don't know data type. Even if some is possible, it's hard to do
	with this system.
*/

#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <cstring>
#include <unistd.h>
using namespace std;

enum{DefNoType,FuncdefNoName,FuncdefNoPair,BackfixNoFront};
size_t tab;

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
			l=r;
			switch(s[r]){
			case '[':
				tmp=parse(s.substr(r+1,SIZE_MAX),&r);
				r+=l+2;
				break;
			case ']':
				if(retPos)*retPos=r;
				return;
			case ';':
				if(list.size()<1)throw DefNoType;
				++r;
				stat=isType;
				continue;
			case '|':
				if(list.size()<1)throw BackfixNoFront;
				tmp.list=list;
				tmp.stat=isList;
				while(list.size()>1)list.pop_back();
				list[0]=tmp;
				++backfix;
				++r;
				continue;
			case '\'':
			{	size_t n=0;
				for(++r;r<s.size()&&s[r]!='\'';++r,++n)if(s[r]=='\\')++r;
				tmp.atom=s.substr(l,r-1);
				tmp.stat=isAtom;
				if(n!=1){
					tmp.atom[0]='\"';
					tmp.atom[tmp.atom.size()-1]='\"';
				}
				++r;
				break;
			}default:
				for(;s[r]!=' '&&s[r]!='\t'&&s[r]!=';'&&s[r]!=']'&&s[r]!='|'&&r<s.size();++r)if(s[r]=='\\')++r;
				tmp.atom=s.substr(l,r-l);			
				tmp.stat=isAtom;
			}
			if(stat==isType){
				if(tmp.stat==isList){
					vector<parse> &typeList=list[list.size()-1-backfix].list;
					vector<parse> &tmpList=tmp.list;
					if(typeList.size()!=tmpList.size())throw FuncdefNoPair;
					for(size_t i=0;i<typeList.size();++i){
						typeList[i].atom=typeList[i].explaiN()+' '+tmpList[i].explaiN();
						typeList[i].stat=isAtom;
					}
				}else{
					def.emplace_back(list[list.size()-1-backfix],tmp.explaiN());
					list[list.size()-1-backfix]=tmp;
				}
				stat=isList;
			}else
				list.insert(list.end()-backfix,tmp);
		}
	}
	string explain(){
		ostringstream s;
		//DEFER: crashes when def.size()>1, no idea why is that
		for(size_t i=0;i<def.size();++i){
			string S=def[i].type->explaiN();
			size_t j;
			for(j=1;j<S.size()&&S[j]!='(';++j);
			if(j<S.size()&&S[j]=='(')s<<S.substr(0,j)<<' '<<def[i].var<<S.substr(j,SIZE_MAX);
			else s<<S<<' '<<def[i].var;
			s<<';';
		}
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
			char bra='(',ket=')',col=',';
			string head=list[0].explaiN();
			if(head=="enum"){
				bra='{';
				ket='}';
			}else if(head=="it"){
				list.erase(list.begin());	
				s<<"for(auto&)"<<'h'+tab<<':'<<explaiN()<<')';
				return s.str();
			}else if(head=="span"){
				list.erase(list.begin());
				char it='h'+tab;
				s<<"for(auto "<<it<<"=0;"<<it<<"<("<<explaiN()<<");++"<<it<<')';
				return s.str();
			}else if(head=="atom"){
				list.erase(list.begin());
				bra=' ';
				ket=' ';
				col=' ';
			}else if(head[0]=='-'||head[0]=='.'||(head[0]>='0'&&head[0]<='9')){
				for(size_t i=0;i<head.size();++i)
					if(head[i]=='.'){
						head="double";
						goto explainOut;
					}else if(head[i]=='\\')
						++i;
				head="long long";
			}
			explainOut:
			s<<list[0].explaiN()<<bra;
			if(list.size()>1){
				s<<list[1].explaiN();
				for(size_t i=2;i<list.size();++i)s<<col<<list[i].explaiN();	
			}
			s<<ket;
			return s.str();
		}else return atom;
	}
	enum{isAtom=0,isList,isType} stat;
	size_t backfix=0;
	vector<parse> list;
	string atom;
};

deF::deF(const parse& a,const string& b):var(b){
	type=new parse;
	type->stat=a.stat;
	type->list=a.list;
	type->atom=a.atom;
}
deF::~deF(){
	delete type;
}

int main(int argc,char** argv){
{	int i;
	size_t j,line;
	size_t &J=tab;
	J=0;
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
				j=0;
				f<<parse(s,&j).explain()<<'\n';
				if(j)
					cerr<<argv[0]<<": "<<argv[i]<<' '<<line<<": Expect no extra ']'\n";
			}catch(int n){
				switch(n){
				case DefNoType:
					cerr<<argv[0]<<": "<<argv[i]<<' '<<line<<": Expected type before ';'\n";continue;
				case FuncdefNoName:
					cerr<<argv[0]<<": "<<argv[i]<<' '<<line<<": Expected function name before '='\n";continue;
				case FuncdefNoPair:
					cerr<<argv[0]<<": "<<argv[i]<<' '<<line<<": Expected atom number aside ';' to be the same\n";continue;
				case BackfixNoFront:
					cerr<<argv[0]<<": "<<argv[i]<<' '<<line<<": Expected function before '/'\n";continue;
				}
			}

		}
		if(J>0){
			f<<';';
			for(;J>0;--J)f<<'}';
		}
	}
}	argv[0]=(char*)"g++";
	execvp(argv[0],argv);
	argv[0]=(char*)"clang++";
	execvp(argv[0],argv);
	argv[0]=(char*)"icl";
	execvp(argv[0],argv);
	cerr<<argv[0]<<": Files converted, but cannot open either gcc, clang, or icc\n";
}		
