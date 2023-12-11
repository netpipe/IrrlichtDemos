#!/bin/bash


#ex -s -c '3i|hello world' -c x file.txt
    #3 select line 3
    #i insert text and newline
    #x write if changes have been made (they have) and exit
#ex -s -c '/hello/i|world' -c x file.txt

#head -n 2 ./file.txt > newfile.txt
#echo "text to insert" >> newfile.txt
#tail -n +3 ./file.txt >> newfile.txt

#awk 'NR==3{print "text to insert"}1' a.txt
#grep -n shell test.txt

sed -i '/pattern/a <script src="FileSaver.js"> <\/script>
    <script>
      function saveFileFromMemoryFSToDisk(memoryFSname,localFSname)     
      {
         var data=FS.readFile(memoryFSname);
         var blob;
         var isSafari = \/^((?!chrome|android).)*safari\/i.test(navigator.userAgent);
         if(isSafari) {
            blob = new Blob([data.buffer], {type: "application\/octet-stream"});
         } else {
            blob = new Blob([data.buffer], {type: "application\/octet-binary"});
         }
         saveAs(blob, localFSname);
      }
    <\/script>' file.html


   <script src="FileSaver.js"> </script>
    <script>
      function saveFileFromMemoryFSToDisk(memoryFSname,localFSname)     // This can be called by C++ code
      {
         var data=FS.readFile(memoryFSname);
         var blob;
         var isSafari = /^((?!chrome|android).)*safari/i.test(navigator.userAgent);
         if(isSafari) {
            blob = new Blob([data.buffer], {type: "application/octet-stream"});
         } else {
            blob = new Blob([data.buffer], {type: "application/octet-binary"});
         }
         saveAs(blob, localFSname);
      }

      function sendHTTP(url,params)     // This can be called by C++ code
      {
			sendForm(url,params);
      }

      function sendFILE(url,params)     // This can be called by C++ code
      {
         var data=FS.readFile(memoryFSname);
         var blob;
         var isSafari = /^((?!chrome|android).)*safari/i.test(navigator.userAgent);
         if(isSafari) {
            blob = new Blob([data.buffer], {type: "application/octet-stream"});
         } else {
            blob = new Blob([data.buffer], {type: "application/octet-binary"});
         }
        // saveAs(blob, localFSname);
		//send to http code possibly using sendForm
		sendForm(url,blob);
      }
