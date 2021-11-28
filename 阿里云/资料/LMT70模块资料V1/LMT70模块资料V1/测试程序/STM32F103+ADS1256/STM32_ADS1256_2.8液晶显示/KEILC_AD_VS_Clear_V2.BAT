@echo off 
cls 
color 0E 
echo ***************************************
echo     COPYRIGHT by badboy 2013
echo ***************************************
echo. & pause
echo 正在清除系统垃圾文件，请稍等......
　　del  /s /q *.lst
　　del  /s /q *.obj
　　del  /s /q *.log
　　del  /s /q *.htm
　　del  /s /q *.PCBPreview
　　del  /s /q *.pcb_viewstate
　　del  /s /q *.PcbDocPreview
　　del  /s /q *.pcbdoc_viewstate
　　del  /s /q *.SchPreview
　　del  /s /q *.netPreview
　　del  /s /q *.SchDocPreview
　　del  /s /q *.PrjPCBStructure
　　del  /s /q *.SchPreview
      del  /s /q  *History*
echo **********************************************************************
      del  /s /q  *.lnp
      del  /s /q  *.Bak
      del  /s /q  *.LST
      del  /s /q  *.OBJ
      del  /s /q  *.M51
      del  /s /q  *.Opt          
      del  /s /q  *.plg
      del  /s /q  *.OutJob
      del  /s /q  *.Dat
      del  /s /q  *.drc
      del /f /s /q  *.htm
      del  /s /q  *.crf
      del  /s /q  *.d
      del  /s /q  *.o
      Rem   del  /s /q  *.axf     
      del  /s /q  *.fed
      del  /s /q  *.tra
      del  /s /q  *.sct
      del  /s /q  *.map
      del  /s /q  *._d
      del  /s /q  *._o
      del  /s /q  *.__i
      del  /s /q  *.ls1
      del  /s /q  *.SRCPreview
      del  /s /q  *.ls1
      del  /s /q  *.i
      del  /s /q  *.lanhong
echo **********************************************************************
　　del  /s /q *.iex
　　del  /s /q *.iex
　　del  /s /q *.__i
　　del  /s /q *._i
　　del  /s /q *.i
　　del  /s /q *.Administrator
　　del  /s /q *.lst
　　del  /s /q *.obj
　　del  /s /q *.log
      del  /s /q  *.lnp
      del  /s /q  *.Bak
      del  /s /q  *.LST
      del  /s /q  *.OBJ
      del  /s /q  *.M51
      del  /s /q  *.Opt             Rem  文件默认打开
      del  /s /q  *.plg
      del  /s /q  *.Dat
      del  /s /q  *.crf
      del  /s /q  *.d
      del  /s /q  *.o
      del  /s /q  *.axf             Rem  文件默认打开
      del  /s /q  *.fed
      del  /s /q  *.tra
      del  /s /q  *.sct
      del  /s /q  *.map
del *.ddk /s
del *.edk /s
del *.lst /s
del *.lnp /s
del *.mpf /s
del *.mpj /s
del *.obj /s
del *.omf /s
del *.opt /s
del *.plg /s
del JLinkLog.txt /s

      del  /s /q  *.htm
      del  /s /q  *.dep
      del  /s /q  *.uvopt

echo **********************************************************************
echo Removing *.$$$ files...
del *.$$$ /f /q /s
echo.

echo Removing *.idb files...
del *.idb /f /q /s
echo.

echo Removing *.manifest files...
del *.manifest /f /q /s
echo.

echo Removing *.pdb files...
del *.pdb /f /q /s
echo.

echo Removing *.user files...
del *.user /f /q /s
echo.

echo Removing *.pch files...
del *.pch /f /q /s
echo.

echo Removing *.bkx files...
del *.bkx /f /q /s
echo.

echo Removing *.cce files...
del *.cce /f /q /s
echo.

echo Removing *.cod files...
del *.cod /f /q /s
echo.

echo Removing *.cof files...
del *.cof /f /q /s
echo.

echo Removing *.err files...
del *.err /f /q /s
echo.

echo Removing *.hex files...
del *.hex /f /q /s
echo.

echo Removing *.i files...
del *.i /f /q /s
echo.

echo Removing *.lde files...
del *.lde /f /q /s
echo.

echo Removing *.lst files...
del *.lst /f /q /s
echo.

echo Removing *.obj files...
del *.obj /f /q /s
echo.

echo Removing *.o files...
del *.o /f /q /s
echo.

echo Removing *.rlf files...
del *.rlf /f /q /s
echo.

echo Removing *.sym files...
del *.sym /f /q /s
echo.

echo Removing *.sdb files...
del *.sdb /f /q /s
echo.

echo Removing *.wat files...
del *.wat /f /q /s
echo.

echo Removing *.mcs files...
del *.mcs /f /q /s
echo.

echo Removing *.mptags files...
del *.mptags /f /q /s
echo.

echo Removing *.tagsrc files...
del *.tagsrc /f /q /s
echo.

echo Removing *.map files...
del *.map /f /q /s
echo.

echo Removing *.ncb files...
del *.ncb /f /q /s
echo.

echo Removing *.suo files...
del *.suo /f /q /s
echo.

echo Removing *.MCHP-MAIN.* files...
del *.MCHP-MAIN.* /f /q /s
echo.

　　del  /s /q *.bsc
　　del  /s /q *.ilk
　　del  /s /q *.sbr
　　del  /s /q *.sbc
　　del  /s /q *.dep

rd Objects /S /Q

rd Debug /S /Q

rd Release /S /Q

echo Removing *.suo files...
del *.suo /f /q /s
echo.

echo **********************************************************************
del /s/f/q *.rpt
del /s/f/q *.cdf 
del /s/f/q *.bak
del /s/f/q *.dfp
del /s/f/q *.pof
del /s/f/q *.a
del /s/f/q *.o
del /s/f/q *.d
del /s/f/q *.ptf-t
del /s/f/q *.x-t
del /s/f/q *.sh-t
del /s/f/q *.mk-t
del /s/f/q *.c-t
del /s/f/q *.mk
del /s/f/q *.eqn
del /s/f/q *.sof
del /s/f/q *.gdb
del /s/f/q *.x
del /s/f/q *.summary
del /s/f/q *.done
del /s/f/q *.smsg
del /s/f/q *.jdi
for /r %%a in (db) do (if exist "%%a" rd/q/s "%%a")
for /r %%a in (incremental_db) do (if exist "%%a" rd/q/s "%%a")
for /r %%a in (simulation) do (if exist "%%a" rd/q/s "%%a")

color 0A 
echo ***************************************
echo 清除系统垃圾完成!!!
echo COPYRIGHT by badboy  2013
echo MODIFY by fzlzdz 2013
echo ***************************************
;echo. & pause

