#! /bin/sh

uploads="_images _static _sources *.html"

full_uploads=""

for i in $uploads; do
	full_uploads="$full_uploads build/html/$i"
done

#echo $full_uploads
scp -r $full_uploads thmalthou@sftp.reed.edu:html/nbody2-docs/
