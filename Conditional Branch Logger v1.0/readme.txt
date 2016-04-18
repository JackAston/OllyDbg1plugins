Conditional Branch Logger (CBL) is a plugin which gives control and logging capabilities for conditional jumps over the full user address space of a process.

From the main OllyDbg plugin menu select Conditional Branch Logger -> Configuration.

This will open a dialog for managing Included (Logged) and Excluded address ranges, conditional branch type selection and project settings. Ranges can be entered manually or selected from one of the Conditional Branch Logger context menu options available in other OllyDbg windows.

Main CPU window (Alt+C): Add multiline selections as Included or Excluded ranges.
Executable modules (Alt+E): Add code section as an Included range. Set ranges by procedure for any module. 
Memory map (Alt+M): Add any memory address region, including non-standard or memory mapped sections, as an Included range. Set ranges by procedure for any module section successfully analyzed.

The Set Ranges by Procedure option, available from the main menu or one of the context menus, will open a window from which individual functions can be added as Included or Excluded ranges for any module. The module will be automatically analyzed via the OllyDbg code analysis function if required.

Each time the main dialog is closed the CBL jump table window will be updated with the latest conditional jump breakpoint addresses. A context menu provides several options for further controlling the selected breakpoints both before and after analysis. When you are satisfied with the settings, run or single-step the target as usual. The results will be shown in the logfile and/or the CBL jump table window. 
A button on the OllyDbg toolbar can be used to show the two Conditional Branch Logger custom windows if they have been hidden or closed.

A logfile can be specified from the main Configuration dialog in which all executed conditional branch instructions within the selected logging range and the result of whether the jump was taken or not are recorded.  If a logfile is not specified a default file named "conditional_branch_logger_default.log" in the OllyDbg main executable directory will be used. Such log files, from different runs of the same program, can then be compared using any good 'diffing' program to find changes in the code execution path as a result of changing inputs or conditions.

The majority of Conditional Branch Logger settings, including active breakpoints, are saved in the OllyDbg UDD project files and restored when the target is reopened. This means that you can log conditional branch instructions in system dlls such as ntdll.dll which occur even before the Entry Point of the target is reached.



The salient features are as follows:

* Ability to detect all conditional branches and log their behaviour during runtime without having to single step the whole process, which results in a dramatic improvement in performance when compared to run trace logging.

* Ability to choose specific conditional branch types to monitor and log.

* Ability to choose and optimize included ranges and excluded ranges to fine tune the logging.

* Ability to disable, delete and restore the logging status of the detected conditional branches.

* Log conditional branches from multiple modules.

* Ability to list all module procedures that OllyDbg has recognized, with their symbolic names if they exist, as a handy reference so that it is easier to include or exclude ranges.

* A text mode log file that could serve to compare two similar runs to detect divergent paths taken with respect to input.

* A runtime log window that updates the status of conditional branches live with context menus to edit, delete and disable the entries on the fly.

* Context menus in Executable modules window to mass add modules after auto analyzing them or add specific modules to be included in the logging.

* Context menus in Memory map window to add any module section, including non-standard or memory mapped regions, to be included in the logging.

* Context menus in the main Disassmebly window to add odd ranges to be included or excluded from the logging.

* Other features include saving the entire database of conditional branches to the OllyDbg UDD file and restored back when restarting the project.

We hope this plugin might prove useful when monitoring execution flow path.


Any comments can be directed to the OllyDbg forums at 
http://www.woodmann.com

This plugin is available at
http://www.woodmann.com/ollystuph/Conditional_Branch_Logger_v1.0.zip

Regards,
Blabberer, dELTA and Kayaker
