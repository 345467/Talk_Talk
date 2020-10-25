#pragma once

class Cfg {
        map<string,string> values;
        pair<string,string> cut(string in,char c){
        	auto it=in.find(c);
        	return make_pair(in.substr(0,it),in.substr(it+1));
		}
    public:
        Cfg(string fname,string default_string=" ") {
        	logger.info("打开文件:"+fname);
            ifstream *fin=new ifstream(fname);
            //if(!fin->good()) {
            if(true){
            	logger.error(fname+"无法打开!");
            	logger.error("正在尝试载入默认设置...");
                delete fin;
                ofstream fout(fname);
                if(!fout.good()){
                	logger.fatal("无法载入默认设置!"); 
                	logger.fatal("程序非正常退出");
					exit(-1);
				} 
                fout<<default_string;
                fout.close();
                fin=new ifstream(fname);
                if(!fin->good()){
                	logger.fatal(fname+"仍然无法打开!");
                	logger.fatal("程序非正常退出");
                	exit(-1);
				}
            }
            while(fin->good()) {
                string line;
                getline(*fin,line);
                if(line.size()==0||line[0]=='#') continue;
                auto k_v=cut(line,'=');
                if(k_v.second==""){
                	logger.info("对键"+k_v.first+"填充默认值:null");
                	k_v.second="null";
				}
				logger.debug("键:"+k_v.first+",值:"+k_v.second);
                values.insert(k_v);
            }
            fin->close();
        }
        string get(string key) {
            if(values.find(key)!=values.end()){
            	logger.debug("查询值:"+key);
            	return values[key];
			}else{
				logger.error("查询值"+key+"失败!");
				logger.error("已返回默认值:null");
            	return "null";
			} 
        }
};
