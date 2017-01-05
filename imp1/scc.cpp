#include <fstream>
#include <cstring>
#include <iostream>
#include <vector>
using namespace std;
int main(int argc,char** argv){
{	int i;
	size_t j,J=0,k,line;
	char it,iT;
	ifstream F;
	ofstream f;
	string s;
	for(i=argc-1;i>0;--i){
		if(argv[i][0]=='-')continue;
		F.open(argv[i]);
		if(!F){
			cerr<<"E "<<argv[i]<<": cannot open such file\n"
			    <<"  note: argument to option must be attached to option, like -oOutput\n";
			continue;
		}
		for(j=strlen(argv[i])-2;argv[i][j]!='.'&&j>0;--j);
		if(j<=0){
			cerr<<"E "<<argv[i]<<": file extension not found\n";
			continue;
		}
		++j;
		if(argv[i][j]<'A'||argv[i][j]>'Z'){
			cerr<<"E "<<argv[i]<<": file extension must have first letter uppercase\n";
			continue;
		}	
		argv[i][j]=tolower(argv[i][j]);
		f.open(argv[i]);
		if(!f){
			cerr<<"E "<<argv[i]<<": cannot open output file\n";
			continue;
		}
		// include all-together header
		f<<"#include\"scc.h\"\n";
		line=0;
		while(getline(F,s)){
			++line;
			// skip macros since they don't follow indentation
			if(s[0]=='#')goto push;
			for(j=0;j<=J&&s[j]=='\t';++j);
			// skip empty line since people can't see them
			if(j==s.size())goto push;
			// handle bracket and semicolumn
			if(j>J)
				for(;j>J;++J)f<<'{';
			else{
				f<<';';
				for(;J>j;--J)f<<'}';
			}
			// handle single-element line
			// this does not do strict checking at all, so tricks can be used (sometimes must be used)
			// * character and string
			if(s[j]=='\''||s[j]=='"'){
				f<<"cout<<"<<s;
				continue;
			}
			// * number
			if(s[j]=='-'||s[j]=='.'||(s[j]>='0'&&s[j]<='9')){
				it=j+'h';
				for(;j<s.size()&&s[j]!=';';++j);
				if(j==s.size()){
					f<<"for(auto "<<it<<"=0;"<<it<<"<("<<s.substr(J,j-J)<<");++"<<it<<")\n";
					continue;
				}
				for(k=j+1;k<s.size()&&s[k]!=';';++k)
				if(k==s.size()){
					f<<"for(auto "<<it<<"=("<<s.substr(j+1,k-j)<<");"<<it<<"<("<<s.substr(J,j-J)<<");++"<<it<<")\n";
					continue;
				}
				f<<"for(auto "<<it<<"=("<<s.substr(j+1,k-j-1)<<");"<<it<<"<("<<s.substr(J,j-J)<<");"<<it<<"+=("<<s.substr(k+1,s.size()-1-k)<<"))\n";
				continue;
			}
			// * variable
			for(;j<s.size()&&(s[j]=='_'||(s[j]>='0'&&s[j]<='9')||(s[j]>='A'&&s[j]<='Z')||(s[j]>='a'&&s[j]<='z'));++j);
			if(j==s.size()){
				it=J+'h';
				iT=J+'H';
				f<<"for(auto "<<it<<'='<<s<<".begin(),"<<iT<<"=0;"<<it<<"!="<<s<<".end();++"<<it<<",++"<<iT<<")\n";
				continue;
			}			
			push:
			f<<s<<'\n';
		}
		f<<';';
		for(;J>0;--J)f<<'}';
	}
	argv[0][0]='g';
}	execvp(argv[0],argv);
}		
