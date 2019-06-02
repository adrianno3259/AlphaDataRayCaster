#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <SimpleArgParser/SimpleArgParser.hpp>

#include "Application/Session.h"


namespace DarkRenderer{

	std::shared_ptr<parser::ArgumentParser> argumentParser;

	namespace arguments
	{
		const std::string file = "--file";
		const std::string output = "--output";
		const std::string use_fpga = "--use-fpga";
	}

	/** Initialize the CLI argument parser from
    *   SimpleArgParser lib. 
    *
    *   @param argc
    *       Number of command line arguments. Redirected from main
    *   @param argv
    *       Array of CLI string arguments. Redirected from main
    */
    void InitParser(int argc, char** argv)
    {
        // Init parser
        argumentParser = 
            std::make_shared<parser::ArgumentParser>(argc, argv);

        // Add input file argument
        argumentParser->addArgument(
            arguments::file,
            true,
            "Path to a .dark input file"
        );

        // Add the output image file argument
        argumentParser->addArgument(
            arguments::output,
            true,
            "Path to a PPM output file."
        );

        argumentParser->addArgument(
            arguments::use_fpga,
            false,
            "Render the image using the FPGA board."
        );
        argumentParser->parse();

    }


	Application::Session session;

	void InitSession()
    {

        if(argumentParser->hasValue(
            arguments::file)
        ) {

            std::string file = 
                argumentParser->getArgument
                    <std::string>(
                        arguments::file
                    );

            std::cout << "LOG: file defined: "
                      << file
                      << std::endl;
            session.readDarkFile(
                file
            );
        }

        if(argumentParser->hasValue(arguments::output))
        {
            std::cout << "LOG: Output name" << std::endl;
            session.outputName = 
                argumentParser->getArgument<std::string>(
                    arguments::output
                );
        }

        session.useFPGA =
            argumentParser->isDefined(
                arguments::use_fpga
            );
    
    }


    /** Next global identifier of the mesh
    */
    int g_rayLastId;

    int g_meshLastId;

    void InitDarkRenderer()
    {
    	// Initialize last ray Id
    	g_rayLastId = 0;


    }



}



#endif