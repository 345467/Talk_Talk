#pragma once

class Cfg {
        map<string,string> values;
        pair<string,string> cut(string in,char c){
        	auto it=in.find(c);
        	return make_pair(in.substr(0,it),in.substr(it+1));
		}
    public:
        Cfg(string fname,string default_string=" ") {
        	logger.info("���ļ�:"+fname);
            ifstream *fin=new ifstream(fname);
            //if(!fin->good()) {
            if(true){
            	logger.error(fname+"�޷���!");
            	logger.error("���ڳ�������Ĭ������...");
                delete fin;
                ofstream fout(fname);
                if(!fout.good()){
                	logger.fatal("�޷�����Ĭ������!"); 
                	logger.fatal("����������˳�");
					exit(-1);
				} 
                fout<<default_string;
                fout.close();
                fin=new ifstream(fname);
                if(!fin->good()){
                	logger.fatal(fname+"��Ȼ�޷���!");
                	logger.fatal("����������˳�");
                	exit(-1);
				}
            }
            while(fin->good()) {
                string line;
                getline(*fin,line);
                if(line.size()==0||line[0]=='#') continue;
                auto k_v=cut(line,'=');
                if(k_v.second==""){
                	logger.info("�Լ�"+k_v.first+"���Ĭ��ֵ:null");
                	k_v.second="null";
				}
				logger.debug("��:"+k_v.first+",ֵ:"+k_v.second);
                values.insert(k_v);
            }
            fin->close();
        }
        string get(string key) {
            if(values.find(key)!=values.end()){
            	logger.debug("��ѯֵ:"+key);
            	return values[key];
			}else{
				logger.error("��ѯֵ"+key+"ʧ��!");
				logger.error("�ѷ���Ĭ��ֵ:null");
            	return "null";
			} 
        }
};
