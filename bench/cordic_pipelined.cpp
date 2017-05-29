#include <verilated.h>          // Defines common routines
#include "Vcordic_pipelined.h"
#include "verilated_vcd_c.h"

#include <assert.h>
#include <math.h>  
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <bitset>
#include <limits>
#include <iomanip>
#include <typeinfo>
#define PI 3.14159265
#define TOLERANCE 0.01

#define X(i) uut-> cordic_pipelined__DOT__stage_generate__BRA__##i##__KET____DOT____Vcellout__stage_i__x_o
#define XOUT(i) cout << "x[" << i << "] = " << Dict::bits_to_double<18>(std::bitset<18>(X(i)).to_string()) << endl 

#define Y(i) uut-> cordic_pipelined__DOT__stage_generate__BRA__##i##__KET____DOT____Vcellout__stage_i__y_o
#define YOUT(i) cout << "y[" << i << "] = " << Dict::bits_to_double<18>(std::bitset<18>(Y(i)).to_string()) << endl 

#define Z(i) uut-> cordic_pipelined__DOT__stage_generate__BRA__##i##__KET____DOT____Vcellout__stage_i__z_o
#define ZOUT(i) cout << "z[" << i << "] = " << Dict::bits_to_double<12>(std::bitset<12>(Z(i)).to_string(), true) << endl 

#define Zin(i) uut-> cordic_pipelined__DOT__stage_generate__BRA__##i##__KET____DOT____Vcellinp__stage_i__z_i

Vcordic_pipelined *uut;                     // Instantiation of module
vluint64_t main_time = 0;       // Current simulation time

class Dict{
    public:
	double sc_time_stamp () {       // Called by $time in Verilog
	    return main_time;           // converts to double, to match
	    // what SystemC does
	}

	double cal_sin(int angle){ 	// Returns the sine of an angle of x radians.
	    printf("angle = %d\n", angle);
	    return sin(angle*PI/180);
	}

	double cal_cos(int angle){ 	// Returns the cosine of an angle of x radians.
	    return cos(angle*PI/180);
	}

	bool check_result(void){

	    int angle = static_cast<int>
			(bits_to_double<13>( std::bitset<13>( uut->z0 ).to_string(), true));    
// since angle starts from 0.0 degree instead of 1.0 degree and we are only getting the final result after 10 positive edges (iterations). z0 is the input angle used to generate this final result. Divide by 16 becuase we have 4 decimal place

	    //cout << "uut->z0 / 16 = " << uut->z0 / 16 << endl; 
	    double cos_result = cal_cos(angle);
	    double sin_result = cal_sin(angle);

	    double dxn = bits_to_double<18>( std::bitset<18>(uut->xn).to_string() );
	    double dyn = bits_to_double<18>( std::bitset<18>(uut->yn).to_string() );	    

	    //std::cout<<"uut->z0 is of type: "<<typeid(uut->z0).name()<<std::endl;
	    //cout << "std::bitset<18>(uut->xn) = " << std::bitset<18>(uut->xn) << endl;   
	    //cout << "std::bitset<18>(uut->yn) = " << std::bitset<18>(uut->yn) << endl;

	    //cout << "to_string(uut->xn) = " << std::bitset<18>(uut->xn).to_string() << endl;   
	    //cout << "to_string(uut->yn) = " << std::bitset<18>(uut->yn).to_string() << endl;
	    
	    cout << "dxn = " << dxn << endl;   
	    cout << "dyn = " << dyn << endl;

	    cout << "cos_result = " << cos_result << endl;   
	    cout << "sin_result = " << sin_result << endl;

	    assert( abs(dxn - cos_result) < TOLERANCE );
	    assert( abs(dyn - sin_result) < TOLERANCE );

	    if (angle == 180) exit(0);   // to stop at +180 degrees

	    return true;
	}

	void cout_debug_msg(unsigned int iteration){
	    cout << "--------------start of debug_message--------------" << endl;
	    cout << "iteration = " << iteration << endl;	  
	    
	    cout_XOUT(iteration); 
	    cout_YOUT(iteration);
	    cout_ZOUT(iteration);

	    cout << "--------------end of debug_message--------------" << endl;	    
	}

	void cout_XOUT(unsigned int iteration){
	    if (iteration == 0) XOUT(0); 
	    if (iteration == 1) XOUT(1); 
	    if (iteration == 2) XOUT(2); 
	    if (iteration == 3) XOUT(3); 
	    if (iteration == 4) XOUT(4); 
	    if (iteration == 5) XOUT(5); 
	    if (iteration == 6) XOUT(6); 
	    if (iteration == 7) XOUT(7); 
	    if (iteration == 8) XOUT(8); 
	    if (iteration == 9) XOUT(9); 	    
	}

	void cout_YOUT(unsigned int iteration){
	    if (iteration == 0) YOUT(0); 
	    if (iteration == 1) YOUT(1); 
	    if (iteration == 2) YOUT(2); 
	    if (iteration == 3) YOUT(3); 
	    if (iteration == 4) YOUT(4); 
	    if (iteration == 5) YOUT(5); 
	    if (iteration == 6) YOUT(6); 
	    if (iteration == 7) YOUT(7); 
	    if (iteration == 8) YOUT(8); 
	    if (iteration == 9) YOUT(9); 	    
	}

	void cout_ZOUT(unsigned int iteration){
	    if (iteration == 0) ZOUT(0); 
	    if (iteration == 1) ZOUT(1); 
	    if (iteration == 2) ZOUT(2); 
	    if (iteration == 3) ZOUT(3); 
	    if (iteration == 4) ZOUT(4); 
	    if (iteration == 5) ZOUT(5); 
	    if (iteration == 6) ZOUT(6); 
	    if (iteration == 7) ZOUT(7); 
	    if (iteration == 8) ZOUT(8); 
	    if (iteration == 9) ZOUT(9); 	    
	}

	// Reference: http://www.cplusplus.com/forum/general/216385/
	template < std::size_t N = 18 > // input: bits of the N-bit significand
	double bits_to_double( const std::string& bits , bool print_z=false)
	{
	    //std::cout << "bits = " << bits << '\n'; 
	    //std::cout << "N = " << N << '\n'; 
	    static_assert( N < std::numeric_limits<signed long long>::digits, "overflow" ) ;

	    static auto denom = 1ULL << (N-1) ;

	    if (print_z == false)
		denom = 1ULL << (N-1) ;
	    else 
		denom = 1ULL << 4 ;

	    std::string mantissa ; // remove non-digit characters
	    for( char c : bits ) 
		if( c == '1' || c == '0' ) 
		{
		   mantissa += c ; 
		   //std::cout << "Loop  , mantissa = " << mantissa << '\n'; 
		}
	    
	    //std::cout << "before, mantissa = " << mantissa << '\n';
	    mantissa.resize( N, '0' ) ; // truncate / append zeroes to make N bits
	    //std::cout << "after , mantissa = " << mantissa << '\n';
	    
	    int sign = ( mantissa[0] == '0' ? 1 : -1 );                            
	    //std::cout << "sign = " << sign << endl;

	    if (sign == -1){		// two-complement 		
		mantissa = std::to_string( std::stol(mantissa) - 1 ) ;
		//std::cout << "sign  , mantissa = " << mantissa << '\n';
	        for( char& c : mantissa ) c = ( c == '0' ? '1' : '0' ) ;
	    }     
	    mantissa[0] == '0';                                           
	    //std::cout << "final , mantissa = " << mantissa << '\n';
	    return sign * double( std::bitset<N>(mantissa).to_ullong() ) / denom ; 
	}
};

int main(int argc, char** argv)
{
    // turn on trace or not?
    bool vcdTrace = false;
    VerilatedVcdC* tfp = NULL;

    Verilated::commandArgs(argc, argv);   // Remember args
    uut = new Vcordic_pipelined;   // Create instance

    uut->eval();

    if (vcdTrace)
    {
        Verilated::traceEverOn(true);

        tfp = new VerilatedVcdC;
        uut->trace(tfp, 99);

        std::string vcdname = argv[0];
        vcdname += ".vcd";
        std::cout << vcdname << std::endl;
        tfp->open(vcdname.c_str());
    }
    uut->z0 = 0x14c0; // 1_0100_1100.0000; // start from -180.0 degree to +180.0 degree
    uut->clk = 0;
    uut->eval();

    unsigned int iteration_stage = 0;

    while (!Verilated::gotFinish())   
    {
	Dict d;

	if ((main_time > 0) && (main_time % 5 == 0)){
            uut->clk = uut->clk ? 0 : 1;       	// Toggle clock with period of 10 main_time units
		    				// first positive clock edge at main_time = 5
	    //printf("Toggling clock at time = %d\n", main_time);
	    uut->eval();            // Evaluate combinatorial logic before and after clock edge transitions
	}

//	if ((main_time >= 11) && (main_time % 10 == 1)){ 	// main_time = 11, 21, 31, 41, 51, ...
//            uut->z0 += 0x010;    	// add 000_0001.0000  which is 1.0 degree when clk = 0
					// first negative clock edge at main_time = 10
	    //printf("Incrementing input test angle");
//	}

	if ((main_time >= 5) && (main_time % 10 == 5)){   // main_time = 5, 15, 25, 35, 45, 55, 65, 75, 85, 95, ...
	    
	    std::string stage = std::to_string(main_time);

	    iteration_stage = (main_time == 5) ? 
		0  :  static_cast<uint>( std::stoi(stage.substr(stage.length()-2, 1)) ) ;

	    //cout << "main_time = " << main_time << endl;   
	    //cout << "iteration_stage = " << iteration_stage << endl;

	    d.cout_debug_msg(iteration_stage);  // print debug message
	}

	if ((main_time >= 95) && (main_time % 10 == 5)){   // main_time = 95, 105, 115, 125, ...
	    
	    bool result_passed = false;  
	
	    // check if the result from final iterations matches closely with exact values of cos(angle) and sin(angle)
	    if (iteration_stage == 9) result_passed = d.check_result();  
	    
	    
	    
	    if (result_passed == true) {
		if (uut->z0 != 0x1FF0) 		// not equal to -1.0 degree
			uut->z0 += 0x010;   	
		else 
			uut->z0 = 0;  	// 0.0 degree
	    }
	}

        if (tfp != NULL)
        {
            tfp->dump (main_time);
        }
 
        main_time++;            // Time passes...
    }

    uut->final();               // Done simulating

    if (tfp != NULL)
    {
        tfp->close();
        delete tfp;
    }

    delete uut;

    return 0;
}
