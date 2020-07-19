* ~~Move getPermissions and getType from sftpdir into SFTPSite, as private methods.~~
* ~~All sftp controls internal in SFTPSite
* ~~One internal sesion that never leaves the class
* ~~only outputs the listings.
* ~~for listings, recreate the model each time, delete/set to null and reinstantiate.
* ~~Download display qabstracttableview
* ~~add private free method to SFTPSite
* ~~SFTPSite::sftp_connection, add error messagebox.
* ~~listdir creates QstringList, serverFileBrowser passes to model.
* Address Edit send path to local explorer
* Address Edit send path to server explorer
* qdrag selects file full path
* downloads


* interface the remote server access so we can support different services
* use windows to store the saved PWs so we can remove openssl
* tidy up mem management
* file explorer column sizing
* remove libssh and use QSSh or something else
* fix naming conventions
* add support for B3, S3, Dropbox(?)
* review architecture .... Just rewrite the whole thing??
* cancel button sizing is wrong
* resizing doesn't remember last window position, needs to follow default windows behaviour
