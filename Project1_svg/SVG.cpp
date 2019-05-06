#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
using namespace std;

#include "SVG.h"

SVG::SVG(string tag_name_){
    tag_name = tag_name_;   //svg��ü ������ tag name�� �Բ� ���� 
    if_selected = 1;    //svg��ü ������ �� ��ü�� select�� 
}

void SVG::set_if_selected(int if_selected_){    //if_selected�� �� set 
    if_selected = if_selected_;
}

int SVG::get_if_selected(){     //if_selected�� �� get 
    return if_selected;
}

string SVG::get_tag_name(){    //tag_name�� �� get 
    return tag_name;
}

void SVG::append(string tag_name_){     //append �Լ� 
    if(if_selected == 1){               //���� select�� ���¶��
        svg_child.push_back(new SVG(tag_name_));    //svg�ڽ��� �߰� 
        if_selected = 2;    //�� �� �ڽ��� if_selected�� 2 
        return ;
    }
    else if(if_selected == 2){  //���� �ڽ��� ���õ� ���� �ִٸ� 
        for(int i=0; i<svg_child.size(); i++){
            svg_child[i]->append(tag_name_);    //�ڽĿ� ���� append ���� 
        }
    }
    else{}  
}

void SVG::select(string tag_name_){     //select�� selectAll�� ���� �Լ� 
    if(if_selected == 1){   //select�� ���¶�� 
        for(int j=0; j<svg_child.size(); j++){  
            if(svg_child[j]->get_tag_name() == tag_name_){
                svg_child[j]->set_if_selected(1);   //tag_name�� ���� �ڽ��� selection���� ����  
            }
        }
        if_selected = 2;    //�ڽ��� if_selected�� 2�� ���� 
        return ;
    }
    else if(if_selected == 2){     //�ڽ��� ���õ� ���¶��  
        int check = 0;
        for(int i=0; i<svg_child.size(); i++){
            if(svg_child[i]->get_if_selected() != 0){
                check = 1;     //�ڽ��� ���õǾ��ٸ�(1 or 2) 
                svg_child[i]->select(tag_name_);    //�ڽĿ� ���� select�Լ� ���� 
            }
            if(check == 0){
                set_if_selected(2); //�ڽ� �� select�� ���� ���ٸ� �ڽ��� if_select�� 2�� �ؼ� selection scope�� �ڽ����� �ٲ�. 
                return;   
            }
        }
    }
    else{}
}

void SVG::remove(){
    if((if_selected == 2) && (svg_child.size() == 0)){  //���� selection�� 2�ε� �ڽ��� ���ٸ� 
        if_selected=1;     //�ڽ��� select�ϰ� ���� 
        return;
    }
    int index=0;
    for(int j=0; j<svg_child.size(); j++){  
        if(svg_child[j]->get_if_selected() == 1){   //�ڽ� �� if_selected�� 1�� ���� ���� ��� 
            index=1;
            break;
        }else if(svg_child[j]->get_if_selected() == 2){ //�ڽ� �� if_selected�� 2�� ���� ���� ��� 
            index=2;
            break;
        }
    }
    if(index == 1 || index == 0){   //index�� 0�� ���� �ڽ��� if_selected�� ��� 0�� ��� (scope�� �ڽ�) 
        for(int j=0; j<svg_child.size(); j++){
            if(svg_child[j]->get_if_selected()==1){
                svg_child.erase(svg_child.begin()+j);   //selection�� 1�� ��ü ��� ���� 
                j--;
            }
        }
        if_selected = 1;    //�ڽ��� ���ŵǰ� �� �θ��� �ڽ��� ���� 
        return ;
    }else if(index == 2){
        for(int j=0; j<svg_child.size(); j++){
        if(svg_child[j]->get_if_selected()==2){
            svg_child[j]->remove();     //�ڽ� �� if_selected�� 2�̸� �� ��ü�� ���� remove�Լ� ���� 
            break;
        }
        } 
    }
}

void SVG::end(){ //remove�� ���� 
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
    if((if_selected==2) && (svg_child.size() == 0)){ //���� scope�� �ڽ������� �ڽ��� �ϳ��� ���� ���  
            for(csvdata_iter = csvdata_.begin(); csvdata_iter!=csvdata_.end(); csvdata_iter++){ //csv���� 1���� ��� data�� ����  
                SVG *temp_SVG = new SVG(tag_name_);
                temp_SVG -> Unique_Id[0] = unique_id_;
                temp_SVG -> Unique_Id[1] = csvdata_iter->first;
                temp_SVG->bounded_csv_index = bounded_csv_index_;
                svg_child.push_back(temp_SVG); // SVG�߰� 
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
                        if(svg_child[k]->tag_name == tag_name_){     //���� �̹� �ִ� �ڷ��� ��� if_detected=1
                        if_detected = 1;
                        break;   
                    }
                    }
                }
            }
            if(if_detected == 1){   //���� �ִ� �ڷ��� ��� ��ŵ 
                continue; 
            }else{  //���� �ڷ��� ��� �߰�  
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
    if((if_selected==2) && (svg_child.size() == 0)){ //���� �������� �ڽ��ε� �ڽ��� ������ update�Ұ��̹Ƿ� ��ŵ 
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
                        if(svg_child[k]->tag_name == tag_name_){  //���� �ִ� �ڷ��� ��� 
                        svg_child[k]->bounded_csv_index = bounded_csv_index_;   //bounded_csv_index�� update�� csv index�� update 
                        svg_child[k]->if_selected = 1;  //update�� ��ü�� ���� 
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

void SVG::exit(map<string, vector<string> > csvdata_, string tag_name_, string unique_id_){ //update�� ���� 
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
                        if(svg_child[k]->tag_name == tag_name_){  //�̹� �ִ� �ڷ��� ��� 
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
                svg_child[j]->attribute_iter = svg_child[j]->attribute.find(attr_name_);  //attr_name�̶�� attribute�� �̹� ������ �ִ��� üũ 
            if(svg_child[j]->attribute_iter != svg_child[j]->attribute.end()){  //�ִٸ� 
                svg_child[j]->attribute_iter -> second = attr_value_;  //�� ���� 
            }else{  //���ٸ�  
                svg_child[j]->attribute.insert(pair<string, string>(attr_name_, attr_value_));   //�Ӽ� �߰� 
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
                
                float float_x_multiplier_ = atof(x_multiplier_.c_str()) * j;    //x_multiplier�� float�� �ٲٰ� index�� ���� 
                float float_y_multiplier_ = atof(y_multiplier_.c_str()) * j;    //y_multiplier�� float�� �ٲٰ� index�� ���� 
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
    
                svg_child[j]->attribute_iter = svg_child[j]->attribute.find("transform");   //transform�̶�� attribute�� ������ �ִ��� üũ  
                if(svg_child[j]->attribute_iter != svg_child[j]->attribute.end()){  //�ִٸ�  
                    svg_child[j]->attribute_iter -> second = str;   //�� ����  
                }else{  //���ٸ�  
                    svg_child[j]->attribute.insert(pair<string, string>("transform", str));    //�Ӽ� �߰�  
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

void SVG::dattr(vector<map<string, vector<string> > > csv_database_, vector<string> field_, string svg_attr_name_, string datum_field_name_){ //mul�� add�� �־����� �ʾ��� ��� 
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
                    
                    if(datum_field_name_ == field_[0]){     //���� unique id�� ���� dattr�� �Ѵٸ� 
                        if(svg_child[i]->attribute_iter != svg_child[i]->attribute.end()){  
                            svg_child[i]->attribute[svg_attr_name_] = svg_child[i]->Unique_Id[1];
                        }else{
                            svg_child[i]->attribute.insert(pair<string, string>(svg_attr_name_, svg_child[i]->Unique_Id[1]));
                        }
                    }else{  //�Ϲ� attribute�� ���� �Ѵٸ� 
                    if(svg_child[i]->attribute_iter != svg_child[i]->attribute.end()){
                        int k=0;
                        for(k=1; k<field_.size(); k++){
                            if(field_[k] == datum_field_name_) break;   //datum_field_name�� field �� �� ��° ���� ã�� k�� ���� 
                        }
                        svg_child[i]->attribute[svg_attr_name_] = csv_database_[svg_child[i]->bounded_csv_index-1][svg_child[i]->Unique_Id[1]][k-1];    //�װ��� �ش��ϴ� value�� ����
                                                                                                    //���� field���� ù��°��� davabase���� 0��° ���� �����ؾ��ϹǷ� k-1�� search 
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
    }else if(index == 0){ //�ڽ��� seletion�� ��� 0�� ��� 
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
                            svg_child[i]->attribute.insert(pair<string, string>(svg_attr_name_, string_result));    //��� ����� string���� �ٲپ ����  
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
    for(int i=0; i<svg_child.size(); i++){      //�ڽĿ� ���� print����  
        svg_child[i] -> print(html_out_);
    }
    fprintf(html_out_,"</%s>",tag_name.c_str(),if_selected);
}



