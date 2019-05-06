#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
using namespace std;

#include "SVG.h"

SVG::SVG(string tag_name_){
    tag_name = tag_name_;   //svg객체 생성시 tag name도 함께 저장 
    if_selected = 1;    //svg객체 생성시 이 객체를 select함 
}

void SVG::set_if_selected(int if_selected_){    //if_selected의 값 set 
    if_selected = if_selected_;
}

int SVG::get_if_selected(){     //if_selected의 값 get 
    return if_selected;
}

string SVG::get_tag_name(){    //tag_name의 값 get 
    return tag_name;
}

void SVG::append(string tag_name_){     //append 함수 
    if(if_selected == 1){               //만약 select된 상태라면
        svg_child.push_back(new SVG(tag_name_));    //svg자식을 추가 
        if_selected = 2;    //그 후 자신의 if_selected는 2 
        return ;
    }
    else if(if_selected == 2){  //만약 자식중 선택된 것이 있다면 
        for(int i=0; i<svg_child.size(); i++){
            svg_child[i]->append(tag_name_);    //자식에 대해 append 실행 
        }
    }
    else{}  
}

void SVG::select(string tag_name_){     //select와 selectAll을 위한 함수 
    if(if_selected == 1){   //select된 상태라면 
        for(int j=0; j<svg_child.size(); j++){  
            if(svg_child[j]->get_tag_name() == tag_name_){
                svg_child[j]->set_if_selected(1);   //tag_name을 가진 자식을 selection으로 변경  
            }
        }
        if_selected = 2;    //자신의 if_selected는 2로 변경 
        return ;
    }
    else if(if_selected == 2){     //자식이 선택된 상태라면  
        int check = 0;
        for(int i=0; i<svg_child.size(); i++){
            if(svg_child[i]->get_if_selected() != 0){
                check = 1;     //자식이 선택되었다면(1 or 2) 
                svg_child[i]->select(tag_name_);    //자식에 대해 select함수 실행 
            }
            if(check == 0){
                set_if_selected(2); //자식 중 select된 것이 없다면 자신의 if_select는 2로 해서 selection scope는 자식으로 바꿈. 
                return;   
            }
        }
    }
    else{}
}

void SVG::remove(){
    if((if_selected == 2) && (svg_child.size() == 0)){  //만약 selection은 2인데 자식이 없다면 
        if_selected=1;     //자신을 select하고 끝냄 
        return;
    }
    int index=0;
    for(int j=0; j<svg_child.size(); j++){  
        if(svg_child[j]->get_if_selected() == 1){   //자식 중 if_selected가 1인 것이 있을 경우 
            index=1;
            break;
        }else if(svg_child[j]->get_if_selected() == 2){ //자식 중 if_selected가 2인 것이 있을 경우 
            index=2;
            break;
        }
    }
    if(index == 1 || index == 0){   //index가 0인 경우는 자식의 if_selected가 모두 0인 경우 (scope는 자식) 
        for(int j=0; j<svg_child.size(); j++){
            if(svg_child[j]->get_if_selected()==1){
                svg_child.erase(svg_child.begin()+j);   //selection이 1인 객체 모두 제거 
                j--;
            }
        }
        if_selected = 1;    //자식이 제거되고 그 부모인 자신이 선택 
        return ;
    }else if(index == 2){
        for(int j=0; j<svg_child.size(); j++){
        if(svg_child[j]->get_if_selected()==2){
            svg_child[j]->remove();     //자식 중 if_selected가 2이면 그 객체에 대해 remove함수 실행 
            break;
        }
        } 
    }
}

void SVG::end(){ //remove와 유사 
    if((if_selected==2) && (svg_child.size() == 0)){
        if_selected=1;   
        return;
    }
    int index=0;
    for(int j=0; j<svg_child.size(); j++){
        if(svg_child[j]->get_if_selected()==1){
            index=1;
        }
    }
    if(index == 1){
        for(int j=0; j<svg_child.size(); j++){
            if(svg_child[j]->get_if_selected()==1){
                svg_child[j]->set_if_selected(0);
            }
        }
        if_selected = 1;
        return ;
        
    }else if(index == 0){
        int no_selected_child = 1;
        for(int j=0; j<svg_child.size(); j++){
            if(svg_child[j]->get_if_selected()==2){
                svg_child[j]->end();
                no_selected_child = 0;
                break; 
            } 
        }
       if(no_selected_child == 1){
            if_selected = 1;
            return;
        }
    }
}

void SVG::enter(map<string, vector<string> > csvdata_, string tag_name_,int bounded_csv_index_, string unique_id_){
    map<string, vector<string> >::iterator csvdata_iter;
    if((if_selected==2) && (svg_child.size() == 0)){ //만약 scope는 자식이지만 자식이 하나도 없을 경우  
            for(csvdata_iter = csvdata_.begin(); csvdata_iter!=csvdata_.end(); csvdata_iter++){ //csv파일 1개의 모든 data에 대해  
                SVG *temp_SVG = new SVG(tag_name_);
                temp_SVG -> Unique_Id[0] = unique_id_;
                temp_SVG -> Unique_Id[1] = csvdata_iter->first;
                temp_SVG->bounded_csv_index = bounded_csv_index_;
                svg_child.push_back(temp_SVG); // SVG추가 
            }
        return; 
    }else{
    int index=0;
    for(int j=0; j<svg_child.size(); j++){
        if(svg_child[j]->get_if_selected()==1){     
            index=1;
            break;
        }else if(svg_child[j]->get_if_selected() == 2){
            index=2;
            break;
        }     
    }
    if(index == 1 || index == 0){
        for(int k=0; k<svg_child.size(); k++){
            svg_child[k]->if_selected = 0;   
        }
        int if_detected = 0;
        for(csvdata_iter = csvdata_.begin(); csvdata_iter!=csvdata_.end(); csvdata_iter++){
            if_detected = 0;
            for(int k=0; k<svg_child.size(); k++){
                if(svg_child[k]->Unique_Id[0] == unique_id_){
                    if(svg_child[k]->Unique_Id[1] == csvdata_iter->first){ 
                        if(svg_child[k]->tag_name == tag_name_){     //만약 이미 있는 자료일 경우 if_detected=1
                        if_detected = 1;
                        break;   
                    }
                    }
                }
            }
            if(if_detected == 1){   //만약 있는 자료일 경우 스킵 
                continue; 
            }else{  //없는 자료일 경우 추가  
                SVG *temp_SVG = new SVG(tag_name_);
                temp_SVG -> Unique_Id[0] = unique_id_;
                temp_SVG -> Unique_Id[1] = csvdata_iter->first;
                temp_SVG->bounded_csv_index = bounded_csv_index_;
                svg_child.push_back(temp_SVG);
            }
        }
        return ;
    }else if(index == 2){
        for(int j=0; j<svg_child.size(); j++){
        if(svg_child[j]->get_if_selected() == 2){
            svg_child[j]->enter(csvdata_, tag_name_, bounded_csv_index_, unique_id_);
        }
        } 
    }
}
}

void SVG::update(map<string, vector<string> > csvdata_, string tag_name_, int bounded_csv_index_, string unique_id_){
    map<string, vector<string> >::iterator csvdata_iter;
    if((if_selected==2) && (svg_child.size() == 0)){ //만약 스코프는 자식인데 자식이 없으면 update불가이므로 스킵 
        return;
    }else{
    int index=0;
    for(int j=0; j<svg_child.size(); j++){
        if(svg_child[j]->get_if_selected()==1){
            index=1;
        }else if(svg_child[j]->get_if_selected() == 2){
            index=2;
            break;
        }
    }
    
    if(index == 1 || index == 0){
        for(int k=0; k<svg_child.size(); k++){
            svg_child[k]->if_selected = 0;   
        }
        int if_detected = 0;
        for(csvdata_iter = csvdata_.begin(); csvdata_iter!=csvdata_.end(); csvdata_iter++){
            if_detected = 0;
            for(int k=0; k<svg_child.size(); k++){
                if(svg_child[k]->Unique_Id[0] == unique_id_){    
                    if(svg_child[k]->Unique_Id[1] == csvdata_iter->first){
                        if(svg_child[k]->tag_name == tag_name_){  //만약 있는 자료일 경우 
                        svg_child[k]->bounded_csv_index = bounded_csv_index_;   //bounded_csv_index를 update할 csv index로 update 
                        svg_child[k]->if_selected = 1;  //update된 객체를 선택 
                        break;   
                    }
                    }
                }
            }
        }
        return ;
    }else if(index == 2){
        for(int j=0; j<svg_child.size(); j++){
        if(svg_child[j]->get_if_selected()==2){
            svg_child[j]->update(csvdata_, tag_name_, bounded_csv_index_, unique_id_);
        }
        } 
    }
}
}

void SVG::exit(map<string, vector<string> > csvdata_, string tag_name_, string unique_id_){ //update와 유사 
    map<string, vector<string> >::iterator csvdata_iter;
    if((if_selected==2) && (svg_child.size() == 0)){
        return;
    }else{
    int index=0;
    for(int j=0; j<svg_child.size(); j++){
        if(svg_child[j]->get_if_selected()==1){
            index=1;
            break;
        }else if(svg_child[j]->get_if_selected() == 2){
            index=2;
            break;
        }
    }
    if(index == 1 || index == 0){
        for(csvdata_iter = csvdata_.begin(); csvdata_iter!=csvdata_.end(); csvdata_iter++){
            int if_detected = 0;
            for(int k=0; k<svg_child.size(); k++){
                if(svg_child[k]->Unique_Id[0] == unique_id_){   
                        if(svg_child[k]->Unique_Id[1] == csvdata_iter->first){
                        if(svg_child[k]->tag_name == tag_name_){  //이미 있는 자료일 경우 
                        svg_child[k]->if_selected = 0;  //if_selected = 0
                        break;
                     }
                    }
                }
            }
        }
        return ;
    }else if(index == 2){
        for(int j=0; j<svg_child.size(); j++){
        if(svg_child[j]->get_if_selected()==2){
            svg_child[j]->exit(csvdata_, tag_name_, unique_id_);
        }
        } 
    }
}    
}

void SVG::cattr(string attr_name_, string attr_value_){ 
    if((if_selected == 2) && (svg_child.size() == 0)){   
        return;
    }
    int index=0;
    for(int j=0; j<svg_child.size(); j++){
        if(svg_child[j]->get_if_selected() == 1){
            index=1;
            break;
        }else if(svg_child[j]->get_if_selected() == 2){
            index=2;
            break;
        }
    }
    if(index == 1){
        for(int j=0; j<svg_child.size(); j++){
            if(svg_child[j]->get_if_selected()==1){
                svg_child[j]->attribute_iter = svg_child[j]->attribute.find(attr_name_);  //attr_name이라는 attribute를 이미 가지고 있는지 체크 
            if(svg_child[j]->attribute_iter != svg_child[j]->attribute.end()){  //있다면 
                svg_child[j]->attribute_iter -> second = attr_value_;  //값 수정 
            }else{  //없다면  
                svg_child[j]->attribute.insert(pair<string, string>(attr_name_, attr_value_));   //속성 추가 
            }   
            }
        }
        return ;
    }else if(index == 2){
        for(int j=0; j<svg_child.size(); j++){
        if(svg_child[j]->get_if_selected()==2){
            svg_child[j]->cattr(attr_name_, attr_value_);
            break; 
        }
        } 
    }else if(index == 0) return;
}

void SVG::tattr(string x_multiplier_, string y_multiplier_){
    if((if_selected == 2) && (svg_child.size() == 0)){   
        return;
    }
    int index=0;
    for(int j=0; j<svg_child.size(); j++){
        if(svg_child[j]->get_if_selected() == 1){
            index=1;
        }else if(svg_child[j]->get_if_selected() == 2){
            index=2;
            break;
        }
    }
    if(index == 1){
        for(int j=0; j<svg_child.size(); j++){
            if(svg_child[j]->get_if_selected()==1){
                string temp_x_multiplier = x_multiplier_;
                string temp_y_multiplier = y_multiplier_;
                
                float float_x_multiplier_ = atof(x_multiplier_.c_str()) * j;    //x_multiplier을 float로 바꾸고 index를 곱함 
                float float_y_multiplier_ = atof(y_multiplier_.c_str()) * j;    //y_multiplier을 float로 바꾸고 index를 곱함 
                stringstream x_stream, y_stream;
                x_stream << float_x_multiplier_;
                temp_x_multiplier = x_stream.str();
                y_stream << float_y_multiplier_;
                temp_y_multiplier = y_stream.str();
                                 
                string str;
                str = "translate(";
                str += temp_x_multiplier;
                str += ", ";
                str += temp_y_multiplier;
                str += ")";
    
                svg_child[j]->attribute_iter = svg_child[j]->attribute.find("transform");   //transform이라는 attribute를 가지고 있는지 체크  
                if(svg_child[j]->attribute_iter != svg_child[j]->attribute.end()){  //있다면  
                    svg_child[j]->attribute_iter -> second = str;   //값 변경  
                }else{  //없다면  
                    svg_child[j]->attribute.insert(pair<string, string>("transform", str));    //속성 추가  
                }
            }
        }
        return ;
    }else if(index == 2){
        for(int j=0; j<svg_child.size(); j++){
        if(svg_child[j]->get_if_selected()==2){
            svg_child[j]->tattr(x_multiplier_, y_multiplier_);
        }
        } 
    }else if(index == 0) return;
}

void SVG::dattr(vector<map<string, vector<string> > > csv_database_, vector<string> field_, string svg_attr_name_, string datum_field_name_){ //mul과 add가 주어지지 않았을 경우 
    if((if_selected == 2) && (svg_child.size() == 0)){
        return ;
    }
    int index = 0;
    if(if_selected == 2){
        for(int i=0; i<svg_child.size(); i++){
            if(svg_child[i]->get_if_selected() == 1) {index = 1; break;}
            else if(svg_child[i]->get_if_selected() == 2) {index = 2; break;}
        }
    }
    if(index == 1){
        for(int i=0; i<svg_child.size(); i++){
               if(svg_child[i]->get_if_selected() == 1){    
                    vector<map<string, vector<string> > >::iterator database_iter;
                    vector<string>::iterator field_iter;
                
                    svg_child[i]->attribute_iter = svg_child[i]->attribute.find(svg_attr_name_);    
                    
                    if(datum_field_name_ == field_[0]){     //만약 unique id에 대해 dattr을 한다면 
                        if(svg_child[i]->attribute_iter != svg_child[i]->attribute.end()){  
                            svg_child[i]->attribute[svg_attr_name_] = svg_child[i]->Unique_Id[1];
                        }else{
                            svg_child[i]->attribute.insert(pair<string, string>(svg_attr_name_, svg_child[i]->Unique_Id[1]));
                        }
                    }else{  //일반 attribute에 대해 한다면 
                    if(svg_child[i]->attribute_iter != svg_child[i]->attribute.end()){
                        int k=0;
                        for(k=1; k<field_.size(); k++){
                            if(field_[k] == datum_field_name_) break;   //datum_field_name이 field 중 몇 번째 인지 찾고 k에 저장 
                        }
                        svg_child[i]->attribute[svg_attr_name_] = csv_database_[svg_child[i]->bounded_csv_index-1][svg_child[i]->Unique_Id[1]][k-1];    //그곳에 해당하는 value를 저장
                                                                                                    //만약 field에서 첫번째라면 davabase에선 0번째 값을 참조해야하므로 k-1로 search 
                    }else{
                        int k=0;
                        for(k=1; k<field_.size(); k++){
                            if(field_[k] == datum_field_name_) break;   
                        }
                        svg_child[i]->attribute.insert(pair<string, string>(svg_attr_name_, csv_database_[svg_child[i]->bounded_csv_index-1][svg_child[i]->Unique_Id[1]][k-1]));
                    }
                 }
               }else continue;
        }
    }else if(index == 0){ //자식의 seletion이 모두 0인 경우 
        return;
    }else if(index == 2){
        for(int i=0; i<svg_child.size(); i++){
            if(svg_child[i]->get_if_selected() == 2){
                svg_child[i]->dattr(csv_database_, field_, svg_attr_name_, datum_field_name_);
            }
        }
    }
}

void SVG::dattr(vector<map<string, vector<string> > > csv_database_, vector<string> field_, string svg_attr_name_, string datum_field_name_, float mul_, float add_){
    if((if_selected == 2) && (svg_child.size() == 0)){
        return ;
    }
    int index = 0;
    if(if_selected == 2){
        for(int i=0; i<svg_child.size(); i++){
            if(svg_child[i]->get_if_selected() == 1) {index = 1; break;}
            else if(svg_child[i]->get_if_selected() == 2) {index = 2; break;}
        }
    }
    if(index == 1){
        for(int i=0; i<svg_child.size(); i++){
               if(svg_child[i]->get_if_selected() == 1){
                    vector<map<string, vector<string> > >::iterator database_iter;
                    vector<string>::iterator field_iter;
                    vector<string> temp;
                    temp = csv_database_[svg_child[i]->bounded_csv_index-1][svg_child[i]->Unique_Id[1]];
                    
                    svg_child[i]->attribute_iter = svg_child[i]->attribute.find(svg_attr_name_);
                    if(datum_field_name_ == field_[0]){
                        if(svg_child[i]->attribute_iter != svg_child[i]->attribute.end()){
                            svg_child[i]->attribute[svg_attr_name_] = atof(svg_child[i]->Unique_Id[1].c_str()) * mul_ + add_;
                        }else{
                            float float_result = atof(svg_child[i]->Unique_Id[1].c_str())* mul_ + add_;
                            string string_result;
                            stringstream s;
    		                s << float_result;
    		                string_result = s.str();
                            svg_child[i]->attribute.insert(pair<string, string>(svg_attr_name_, string_result));    //계싼 결과를 string으로 바꾸어서 저장  
                        }
                    }else{
                    if(svg_child[i]->attribute_iter != svg_child[i]->attribute.end()){
                        int k=0;
                        for(k=1; k<field_.size(); k++){
                            if(field_[k] == datum_field_name_) break;   
                        }
                        float float_result = atof(temp[k-1].c_str())* mul_ + add_;
                        string string_result;
                        stringstream s;
		                s << float_result;
		                string_result = s.str();
                        svg_child[i]->attribute[svg_attr_name_] = string_result;
                    }else{
                        int k=0;
                        for(k=1; k<field_.size(); k++){
                            if(field_[k] == datum_field_name_) break;   
                        }
                        float float_result = atof(temp[k-1].c_str())* mul_ + add_;
                        string string_result;
                        stringstream s;
		                s << float_result;
		                string_result = s.str();
                        svg_child[i]->attribute.insert(pair<string, string>(svg_attr_name_, string_result));
                    }
                 }
               }else continue;
        }
    }else if(index == 0){
        return;
    }else if(index == 2){
        for(int i=0; i<svg_child.size(); i++){
            if(svg_child[i]->get_if_selected() == 2){
                svg_child[i]->dattr(csv_database_, field_, svg_attr_name_, datum_field_name_, mul_, add_);
            }
        }
    }
}
/*
void SVG::print(){
    printf("<%s",tag_name.c_str());
    for(attribute_iter = attribute.begin(); attribute_iter != attribute.end(); attribute_iter++){
        printf(" %s = \"%s\" ",attribute_iter -> first.c_str(), attribute_iter -> second.c_str());
    }
    printf(">");
    for(int i=0; i<svg_child.size(); i++){
        svg_child[i] -> print();
    }
    printf("</%s>",tag_name.c_str(),if_selected);
}
*/
void SVG::print(FILE *html_out_){
    fprintf(html_out_,"<%s",tag_name.c_str());
    for(attribute_iter = attribute.begin(); attribute_iter != attribute.end(); attribute_iter++){
        fprintf(html_out_," %s = \"%s\" ",attribute_iter -> first.c_str(), attribute_iter -> second.c_str());       //attribute print
    }
    fprintf(html_out_,">");
    for(int i=0; i<svg_child.size(); i++){      //자식에 대해 print수행  
        svg_child[i] -> print(html_out_);
    }
    fprintf(html_out_,"</%s>",tag_name.c_str(),if_selected);
}



