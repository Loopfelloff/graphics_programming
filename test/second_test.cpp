#include<iostream>
#include<vector>
#include<cmath>

void return_something(std::vector<int>& x_val , std::vector<int>& y_val , int start_x , int finish_x , int start_y , int finish_y){
    
    int dx = finish_x - start_x;
    int dy = finish_y - start_y;
    int decision_param = 2*dy - dx;
    int x_init = start_x;
    int y_init = start_y;
    while(x_init <= finish_x){
	x_val.push_back(x_init);
	y_val.push_back(y_init);
	if (decision_param  < 0){
	    x_init += 1;
	    decision_param  = decision_param + 2 * dy;
	}
	else {
	    x_init += 1;
	    y_init += 1;
	    decision_param = decision_param + 2*dy - 2*dx;
	}
    }
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
    return_something(x_val,  y_val , start_x , finish_x , start_y , finish_y);

    for(int i =0 ; i<x_val.size() ; i++){
	float x_to_push = float(x_val[i])/100 * 2 -1 ;
	float y_to_push = float(y_val[i])/100 * 2 -1 ;
	norm_x_val.push_back(x_to_push);
	norm_y_val.push_back(y_to_push);
    }
    for (int i =0; i <x_val.size(); i++){
	std::cout<<norm_x_val[i]<<"\t"<<norm_y_val[i]<<"\n";
    }
    return 0;
}

