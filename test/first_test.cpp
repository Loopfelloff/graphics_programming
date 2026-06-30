#include<iostream>
#include<vector>
#include<cmath>

int return_something(std::vector<int>& x_val , std::vector<int>& y_val , int start_x , int finish_x , int start_y , int finish_y){
    int dx = finish_x - start_x;
    int dy = finish_y - start_y;
    int steps = std::max(abs(dx) , abs(dy));
    float x_increment = (float)dx/steps;
    float y_increment = (float)dy/steps;
    int x_init = start_x;
    int y_init = start_y;
    for (int i=0; i<=steps ; i++){
	x_val.push_back(x_init);
	y_val.push_back(y_init);
	x_init = std::round(x_init + x_increment);
	y_init = std::round(y_init + y_increment);
    }
    return steps;	
}

int main(){
    std::vector<int> x_val;
    std::vector<int> y_val;
    std::vector<float> norm_x_val;
    std::vector<float> norm_y_val;
    int start_x = 0;
    int start_y = 0;
    int finish_x = 100;
    int finish_y = 100;
    int steps = return_something(x_val,  y_val , start_x , finish_x , start_y , finish_y);

    for(int i =0 ; i<x_val.size() ; i++){
	float x_to_push = float(x_val[i])/steps * 2 -1 ;
	float y_to_push = float(y_val[i])/steps * 2 -1 ;
	norm_x_val.push_back(x_to_push);
	norm_y_val.push_back(y_to_push);
    }
    for (int i =0; i <x_val.size(); i++){
	std::cout<<norm_x_val[i]<<"\t"<<norm_y_val[i]<<"\n";
    }
    return 0;
}
