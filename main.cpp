#include <iostream>

#include "DarkRenderer/Globals.h"

#include "Application/Session.h"

#include "Tracer/Tracer.h"

#include "Chrono.h"

int main(int argc, char** argv){

    Application::Session& sess = DarkRenderer::session;

    DarkRenderer::InitParser(argc, argv);

    DarkRenderer::InitSession();

    Chrono chronometer = Chrono(std::cout);

    std::shared_ptr<Tracer> tracer;

    if(!DarkRenderer::session.useFPGA)
    {
        tracer = std::make_shared<CPUTracer>(sess);
    }
    else
    {
        #ifdef USE_FPGA

        tracer = std::make_shared<FPGATracer>(sess);
        
        #else
        
        std::cerr << "ERROR: FPGA usage not compiled in this version:\n"
                  << "Please recompile with the command line option -DFPGA or run using\n"
                  << "the CPU instead.\n\n";

        exit(EXIT_FAILURE);
        
        #endif

    }
    
    chronometer.start();
    tracer->render(0);
    std::cout 
        << "Finished rendering " << DarkRenderer::session.outputName 
        << " with resolution " 
        << sess.camera->getHorizontalResolution() <<"x"
        << sess.camera->getVerticalResolution() << "\n";
    chronometer.stop("Total render time");
        
    std::cout << "Saving " << DarkRenderer::session.outputName <<"\n";
    if(sess.frames.size() > 0)
        sess.frames[0].save(sess.outputName.c_str());

    return 0;
}
