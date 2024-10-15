12/29/00


an986.irx (Release 2.1.2 version)  
================================= 

This is the release of the USB-Ethernet I/F driver an986.irx (Release 2.1.2 version).

Use of an986.irx within a title is prohibited.
Refer to the following tech note for details.

 - Restrictions on using the an986.irx USB-Ethernet I/F driver (11/22/00)
 
As a result, an986.irx is not included in the Release 2.1.2 package and is being released as a separate package.

The following sample programs included in Release 2.1.2 require an986.irx when using a USB ether adaptor

  Samples using an986.irx:
    ee/sample/inet/ball_game
    ee/sample/inet/echo_server
    ee/sample/inet/http_get
    ee/sample/inet/load_test
    iop/sample/inet/http_get
    iop/sample/inet/load_test


Installation
------------

   Copy an986.irx to /usr/local/sce/iop/modules/ 

   (Example)
   # mv an_212.tgz /tmp
   # cd /tmp
   # tar zxvf an_212.tgz
   # cp -avf an986.irx /usr/local/sce/iop/modules/

