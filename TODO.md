Move getPermissions and getType from sftpdir into SFTPSite, as private methods.
All sftp controls internal in SFTPSite
One internal sesion that never leaves the class
only outputs the listings.
for listings, recreate the model each time, delete/set to null and reinstantiate.
download display qabstracttableview