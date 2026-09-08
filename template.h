#pragma once

class Template{
private:

	// passXML file path + SR to python file. 
	// read from the arrays written to data_files by parsing through the file path in the same way as python
	// get templates setup and stored to where we can send publically accessible reference as span to hmm
	

public:
	Template (
		const std::filesystem::path& musicXmlPath,
		double SR, size_t buffer_size, size_t hopsize
	);


	

}

