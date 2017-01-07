package main

type code interface{
	parse([]byte) uint64
	explain() []byte
}

type def struct{
	name []byte
	type *def
}

type leaf []byte
func (a leaf) parse(t []byte) uint64{
	I:=0
	if (t[0]<='9'&&t[0]>='0')||t[0]=='_'{
		nodot:=true
		for _,i:=range t[1:]{
			++I
			if i>'9'||i<'0'{
				if i=='.'&&nodot{
					nodot=false
				}else{
					break
				}
			}
		}
	}else if (t[0]>='a'&&t[0]<='z')||(t[0]>='A'&&t[0]<='Z')||t[0]=='_'{
		for _,i:=range t[1:]{
			++I
			if i<'0'||(i>'9'&&i<'A')||(i>'Z'&&i<'_')||(i>'_'&&i<'a')||i>'z'{
				break
			}
		}
	}
	a=t[:I]
	return I
}
func (a leaf) explain() []byte{return []byte(atom)}

type tree struct{
	a []code
	d []def
}
func (a tree) parse(t []byte) uint64{
	//TODO
}
func (a tree) explain() (r []byte){
	if len(a.code)<1{
		return ""
	}
	r=a.code[0].explain()
	append(r,"(")
	if len(a.code)>1{
		append(r,a.code[1].explain())
		for _,i:=range t[2:]{
			append(r,",")
			append(r,i.explain())
		}
	}
	append(r,")")
}

func main(){}