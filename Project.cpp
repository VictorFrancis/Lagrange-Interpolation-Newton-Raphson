#include<iostream>

using namespace std;

//horner used for triggering Newton-Raphson Root finding
float horner(float polyCo[], float x, int num){

	float horn=0;

	for(int i=0; i<num; i++){
		horn=(horn*x)+polyCo[i];
	}

	return horn;
}

//Lagrange Interpolation used for finding Co-effecients of polynomials and computing New generated data points
float lagrangeIntpol(int tableX[], int tableY[], float newDataX, int numberOfData){

    float newDataY = 0; //newly generated Y-value

    for(int i=0; i<numberOfData; i++){

        float y = tableY[i];

        for(int j=0; j<numberOfData; j++){
            if(j!=i){
                y = y*(newDataX - tableX[j])/(tableX[i] - tableX[j]); //Lagrange formula
            }                                                         //Ln=y1*(x-x2)*(x-x3)/(x1-x2)*(x1-x3)
        }

        newDataY = newDataY + y; //updating generate Y-value
    }

    return newDataY;
}
//used for computing derivatives of a function
float taylorDiff(int i, float polyCo){

    return i*polyCo; //Performs dy/dx
}

//used for finding root
float newtonRaph(float nextVal, float polyCo[], float polyCoDx[], int numberOfPoints){

	for(int i=0; i<numberOfPoints-1; i++){
		nextVal=nextVal-horner(polyCo,nextVal,numberOfPoints-1)/horner(polyCoDx,nextVal,numberOfPoints-1);
		//formula for computing next new x-value
		//Xn+1=Xn-(f(Xn)/f'(Xn))
	}

	return nextVal;
}

//used for finding Stationary points
void findStationaryPt(int s, int tableX[], int tableY[], int numberOfPoints, float polyCo[], float polyCoDx[], float stPtX[5], float stPtY[5]){

    float currStPtX=0;
    int o=0;

    for(int i=0; i<numberOfPoints-1; i++){
        if(polyCoDx[i]==0){ //stationary points lies where dy/dx=0
            stPtX[o]=polyCo[i]; //storing stationary point
            o++;
        }
    }

    for(int i=0; i<s; i++){
        currStPtX=stPtX[i]; //Temporary variable to pass current stationary point
        stPtY[i]=lagrangeIntpol(tableX,tableY,currStPtX,numberOfPoints); //compute Y-value of the stationary points
        currStPtX=0; //reseting current stationary point
    }

}

void findExtrema(int s, float maxima[5], float minima[5], int numberOfPoints, float stPtX[], float stPtY[], int tableX[], int tableY[]){

    float prevalue; //left neighbouring points of stationary points
    float nextvalue; //right neighbouring points of stationary points
    float prevalueY; //Y-value of left neighbouring points of stationary points
    float nextvalueY; //Y-value of right neighbouring points of stationary points
    int m, mn;

    for(int i=0; i<s; i++){
        prevalue=stPtX[i]-0.5; //left neighbouring points of stationary points
        nextvalue=stPtX[i]+0.5; //right neighbouring points of stationary points
        prevalueY=lagrangeIntpol(tableX,tableY,prevalue,numberOfPoints);
        nextvalueY=lagrangeIntpol(tableX,tableY,nextvalue,numberOfPoints);

        if(prevalueY<stPtY[i] && nextvalueY<stPtY[i]){ //case condition to check if function is  increasing
            maxima[m]=stPtY[i];
            m++;
        }

        else if(stPtY[i]>prevalueY && nextvalueY>stPtY[i]){ //case condition to check if function is  decreasing
            minima[mn]=stPtY[i];
            mn++;
        }
    }

}

//used for finding Absolute Minima Value
    float findAbsoMin(int s, float minima[]){
        float absMin;
        for(int i=0; i<s; i++){
            if(absMin>minima[i]){
                absMin=minima[i];
            }
        }

        return absMin;
    }

//used for finding Absolute Maxima Value
    float findAbsoMax(int s, float maxima[]){
        float absMax;
        for(int i=0; i<s; i++){
            if(absMax<maxima[i]){
                absMax=maxima[i];
            }
        }
    }



int main(){

    int numberOfPoints; //number of Data Points
    float stPtX[10]; //X-value of stationary points
    float stPtY[10]; //Y-value of stationary points
    float maxima[5]; //All Maxima
    float minima[5]; //All Minima

    cout<<"Enter number of Data Points: "<<endl;
    cin>>numberOfPoints;

    int tableX[numberOfPoints]; //All X-value of Input Data points
    int tableY[numberOfPoints]; //All Y-value of Input Data points

    for(int i=0; i<numberOfPoints; i++){
        cout<<"Enter Data points (x y): ";
        cin>>tableX[i]>>tableY[i];
    }

    float newGenDataX[numberOfPoints-1]; //new generated data points from interpolation
    float polyCo[numberOfPoints-1]; //Coeffecients of the polynomial
    float polyCoDx[numberOfPoints-1]= {0}; //Degree of polynomial's X or power


    for(int i=0; i<numberOfPoints-1; i++){
        newGenDataX[i]=(tableX[i+1]+tableX[i])/2; //calculate points between data points
    }

    float currNewDataX; //Temporary variable for passing points to Lagrange

    for(int i=0; i<numberOfPoints-1; i++){
        currNewDataX=newGenDataX[i];
        polyCo[i]=lagrangeIntpol(tableX,tableY,currNewDataX,numberOfPoints);
        currNewDataX=0;
    }

    float currPolyCo=0; //Temporary variable for passing points to Taylor Differentiation

    for(int i=1; i<numberOfPoints-1; i++){
        currPolyCo=polyCo[i];
        polyCoDx[i-1]=taylorDiff(i,currPolyCo);
        currPolyCo=0;

    }

    int s=5; //number of stationary points

    float initVal=0.01; //initial value for Newton-Raphson for finding root

	float nextVal=initVal; //variable for nex computed value for Newton-Raphson for finding root

    float root; //root value

	root=newtonRaph(nextVal,polyCo,polyCoDx,numberOfPoints);
	cout<<"Root: "<<root<<endl;

	findStationaryPt(s,tableX,tableY,numberOfPoints,polyCo,polyCoDx,stPtX,stPtY);

	for(int i=0; i<s; i++){
        cout<<"Stationary points are: "<<"("<<stPtX[i]<<","<<stPtY[i]<<")"<<endl;
	}

    findExtrema(s,maxima,minima,numberOfPoints,stPtX,stPtY,tableX,tableY);

    for(int i=0; i<s; i++){
        cout<<"Extrema points at x = "<<stPtX[i]<<endl;
        cout<<"Extrema points at x = "<<maxima[i]<<endl;
        cout<<"Extrema points at x = "<<minima[i]<<endl;
        cout<<"Extrema points at x = "<<root<<endl;

    }

    for(int i=0; i<s; i++){
        cout<<"Maxima points at x = "<<maxima[i]<<endl;;
    }

    for(int i=0; i<s; i++){
        cout<<"Minima points at x = "<<minima[i]<<endl;
    }


    cout<<"Absolute Minima: "<<findAbsoMin(s,minima)<<endl;
    cout<<"Absolute Maxima: "<<findAbsoMax(s,maxima)<<endl;


return 0;
}


