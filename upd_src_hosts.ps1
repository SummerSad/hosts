$list = @("http://someonewhocares.org/hosts/zero/hosts",
        "http://winhelp2002.mvps.org/hosts.txt")

for($i = 0; $i -lt $list.length; ++$i){
        Invoke-WebRequest -OutFile .\hosts\raw -Uri $list[$i]
        Get-Content .\hosts\raw | Out-File -Encoding ASCII .\hosts\$i.txt
        Remove-Item .\hosts\raw
}

.\merge_host.exe raw .\hosts\1.txt