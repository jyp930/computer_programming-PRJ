#ifndef SVG_H_
#define SVG_H_

class SVG{
    public:
        vector<SVG*> svg_child; //svg자식을 저장할 수 있는 벡터 
        SVG(string tag_name_);
        void set_if_selected(int if_selected_);
        int get_if_selected();
        int find_if_selected();
        string get_tag_name();
        void append(string tag_name_);
        void select(string tag_name_);
        void remove();
        void end();
        void enter(map<string, vector<string> >, string tag_name_, int bounded_csv_index_, string unique_id_);
        void update(map<string, vector<string> >, string tag_name_, int bounded_csv_index_, string unique_id_);
        void exit(map<string, vector<string> >, string tag_name_, string unique_id_);
        void cattr(string attr_name_, string attr_value_);
        void tattr(string x_multiplier_, string y_multiplier_);
        void dattr(vector<map<string, vector<string> > >, vector<string> field, string svg_attr_name_, string datum_field_name_);
        void dattr(vector<map<string, vector<string> > >, vector<string> field, string svg_attr_name_, string datum_field_name_, float mul_, float add_);
        void print(FILE *html_out_);
        //void print();
    private:
        string tag_name;    //tag_name을 저장 
        int if_selected;    //select가 된 상태라면 1, 자식svg가 select된 상태라면 2 
        int bounded_csv_index;  //bound된 csv의 index 
        string Unique_Id[3];    //Unique_Id[0]에는 CSV Unique id의 field name, Unique_Id[1]에는 CSV Unique id의 값 
        map<string, string> attribute;  //svg의 attribute를 저장하는 map 
        map<string, string>::iterator attribute_iter;
};
#endif

