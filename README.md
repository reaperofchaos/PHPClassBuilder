# PHPClassBuilder
Reads a PHP class file with only the class name and properties and generates
a new PHP class with constructor and CRUD methods. 
<br />

# Description
This project is designed to speed up PHP development when creating model classes. As long an input PHP file is provided with a class name, properties and a table_name property that the CRUD operations will be interacting with, generic functions will be created to Insert, Select, Update and Delete records in that table. Generated functions commented in style that will be easily recognized by phpDocumentor. 
<br />

# Usage
The code is run using the command line <br />

<pre>
./PHPClassBuilder -l &lt;input filename&gt;
</pre>

<pre>


<b>-l</b> [FILENAME]        inputFile
                     REQUIRED
                     The input file extension must be a .php or
                     .PHP file or it will not run. 
</pre>

# Build Instructions
## CMAKE
The CodeGenerator file can be compiled using cmake <br />
1. Make a build folder
<pre>
mkdir build
cd build
</pre>
2. Run CMAKE
<pre>
cmake ../src
cmake --build .
</pre>
  
## Make
I also have a simple Make file included that will store the executable in the build folder and any objects needed to build the file in the objs folder
<pre>
make cmd 
make run
</pre>


