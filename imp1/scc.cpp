#include <fstream>
#include <iostream>
#include <vector>
using namespace std;
struct deF{
	deF(parse t,string v):type(t),var(v){}
	parse type;
	string var;
};
vector<deF> def;
struct parse{
	parse(){}
	parse(string s){
		size_t l,r=0;
		while(r<s.size()){
			for(;s[r]==' '||s[r]=='\t';++r);
			l=r;
			switch(s[r]){
			case '[':
				size_t bra=1;
				for(;bra>0&&r<s.size();++r)
					switch(s[r]){
					case '[':++bra;break;
					case']':--bra;break;
					case'\'':for(++r;s[r]!='\''&&r<s.size();++r)if(s[r]=='\\')++r;
					}
				list.emplace_back(s.substr(l+1,r-l-1),err);
				break;
			case '\'':
				bool str=false;
				if(s[++r]=='\''||(s[r]=='\\'&&s[r+2]!='\'')||(s[r]!='\\'&&s[r+1]!='\''))str=true;
				for(;s[r]!='\''&&r.size();++r)if(s[r]=='\\')++r;
				prase content;
				content.lead=str?'"'+s.substr(l+1,r-l-1)+'"':s.substr(l,r-l+1);
				list.push_back(content);				
				break;
			case ';':
				if(list.size()<1)throw EnoTypeBeforeVar;
				for(;s[r]!=' '&&s[r]!='\t'&&r<s.size();++r);
				def.emplace_back(list[list.size()-1],s.substr(l+1,r-l-1));
				list.resize(list.size()-1);
			default:
				for(;s[r]!=' '&&s[r]!='\t'&&s[r]!=';'&&r<s.size();++r);
				parse content;
				content.lead=s.substr(l,r-1);
				if(content.lead=="enum"){
					//TODO
				}
				list.push_back(content);
			}
		}
	}
	string explain(){
		ostringstream s;
		for(size_t i=0;i<def.size();++i){
			string S=def[i].type.explaiN();
			size_t j;
			for(j=1;j<S.size()&&S[j]!='(';++j);
			if(S[j]=='(')s<<S.substr(0,j)<<' '<<def[i].var<<S.substr(j,S.size()-j);
			else s<<S<<' '<<def[i].var;
			s<<';';
		}
		s<<explaiN();
		return s.str();
	}
	string explaiN(){
		if(lead.size()>0)return lead;
		ostringstream s;
		s<<list[0].explaiN()<<'(';
		if(s.size()>1){
			for(size_t i=1;i<list.size()-1;++i)s<<list[i].explaiN()<<',';	
			s<<list[list.size()-1].explaiN();
		}
		s<<')';
		return s.str();
	}
	vector<parse> list;
	string lead;
}
int main(int argc,char** argv){
{	int i;
	size_t j,line;
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
				f<<';'
				for(;J>j;--J)f<<'}';
			}
			try f<<parse(s).explain();catch(int n)switch n{
			case EnoTypeBeforeVar:
				cerr<<argv[0]<<": "<<argv[i]<<' '<<line<<": Expect type before var\n";
				continue;
			}

		}
		for(;J>0;--J)f<<'}';
	}
}	argv[0]="g++"
	execvp(argv[0],argv);
	argv[0]="clang++"
	execvp(argv[0],argv);
	argv[0]="icl"
	execvp(argv[0],argv);
	cerr<<argv[0]<<": Cannot open either gcc, clang, or icc\n";
}		
