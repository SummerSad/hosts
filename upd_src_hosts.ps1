$url = "http://someonewhocares.org/hosts/zero/hosts"

Invoke-WebRequest -OutFile .\hosts\raw -Uri $url
Get-Content .\hosts\raw | Out-File -Encoding ASCII .\hosts\1.txt
Remove-Item .\hosts\raw

.\merge_host.exe raw .\hosts\1.txt