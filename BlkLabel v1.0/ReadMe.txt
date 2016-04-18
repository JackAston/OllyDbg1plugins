Bulk Labelling PlugIn for OllyDbg v1.0
--------------------------------------

There are 8 files in this package, including this ReadMe.

1. BlkLabel.dll ) Copy to OllyDbg
2. SubLabel.dll ) Folder to obtain Clarion MAP
3. BlkLabel.chm ) PlugIn functionality

Create a SubLabel.dll to obtain specific functionality for your Programming Language IDE Output. The specificalion of
SubLabel.dll is set out in BlkLabel.chm. In essence all that is required are two Exports:

MAPFilePerCharacterHandler ... which will receive each Character read (as a 1 Character C-String). (It will also receive,
via a similar 1-Character C-String, the Previous Character ... this may, or may not, be of use ... depends on circumstances).
If Character translation is necessary, overstore the Current Character with a translation (Unicode is NOT supported here). In
this case of SubLabel.dll as supplied, the only translation performed is to turn Tab Character (09h) into a single Space
(20h).

The record that is built, via MAPFilePerCharacterHandler, is - when completed - handed over to:

MAPFilePerRecordHandler  ... from which Label-Address pairs can be extracted any-which-way you want. Use OllyDbg
"_Insertname" to insert them.

4. BlkLabel.clw is the Clarion Source Text of the main PlugIn.
5. SubLabel.clw is the Clarion Source Text that supports a Clarion Memory Map file.

(Being written in Clarion they should be perfectly readable, but will probably be useless to you. This is, of course,
precisely the problem *I* have, IN REVERSE, with ALL examples supplied by other people ... unless the functionality is
described in non-specific/universal terms ... as I have tried to do here).

6. Veronica.obj is my Asssembler-coded stuff that provides 'interfacing glue' between C-Style and Clarion-Style, comprising
   such Functions as SaveRegisters(), RestoreRegisters(), StringCopy(), RemoveLeadingSpaces(), RemoveTrailingSpaces(), etc.
7. Veronica.clw is a Source File Text which declares the Prototypes of the Exports of Veronica.obj.

This PlugIn is really only useful to those who write their own software, using an IDE that can create a Memory Map. In this
case you would find it very useful to be able to transfer your Symbols into OllyDbg. Things become much easier to find!

(As far as I can see) The only thing necessary would be to create a SubLabel.dll - as explained above - to decipher RECORDS
presented sequentially from the Memory Map of your choice ... which should not be a big job. BlkLabel itself does all the
rest.

Contact me on Clarion@VeronicaChapman.com







