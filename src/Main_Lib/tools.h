
#ifndef TOOLS // include guard
#define TOOLS
int saturation_under(int value, int saturation_value){
    if(value < saturation_value ){

        value = saturation_value;
    }

    return value ; 

}

int saturation(int value,int saturation_value){
    if(value>saturation_value){
        value = saturation_value ;
    }
    else if( value < -saturation_value){
        value = -saturation_value ;
    }

    return value;


}
#endif