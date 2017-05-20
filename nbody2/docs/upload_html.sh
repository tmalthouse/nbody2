#! /bin/sh

dirs="_images _static _sources"

sshpass -fpass scp build/html/*.html thmalthou@sftp.reed.edu:html/nbody2-docs/

for d in $dirs; do
	sshpass -fpass scp -r build/html/$d thmalthou@sftp.reed.edu:html/nbody2-docs/
done
