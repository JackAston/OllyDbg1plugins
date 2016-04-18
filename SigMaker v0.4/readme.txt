Author: P47R!CK
Version: 0.4
Support: http://forum.tuts4you.com/index.php?showtopic=25196

This plugin is meant to help you create signatures. You can choose from a variety of styles to suit your needs and you can now also test your signatures.

The supported styles are:

* Code style ( to use with dwFindPattern )
* PB Style ( the style that punkbuster uses )
* Tabris style ( for further info click HERE)
* Text style ( used by Olly, IDA and Op Searcher )

There is also the option to get an offset from the current address which will look like this:

* modulename + offset

clearly you will have to use GetModuleHandle or LoadLibrary to convert the name into an address....

Here is what the options do:

"include short jumps" this option will include short jumps as such as "jmp 10" ( EB 0A )
"Include data" adds data as such as the 5 of mov eax, 5
"include eip related data" will add info that modifies the program flow and stack pointer such as "retn C"
"Add additional info" adds information about the module in which your signature is situated in
"include relative addresses" will add relative offsets to your signature as such as the 8 in this instruction "mov eax,[ecx+8]"
"include unsafe data" is currently disabled because my register tracking code( to determine if the constant is an address ) doesn't work yet.

The last 2 options should be self-explanatory.

The only thing I should mention is that you can double click the results to jump the associated address.