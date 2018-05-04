$list = @("http://winspyblocker.crazyws.fr/data/hosts/win10/spy.txt")
$data = "data"

[Net.ServicePointManager]::SecurityProtocol =  [System.Security.Authentication.SslProtocols] "tls, tls11, tls12"

if(!(Test-Path -PathType Container $data)){
        New-Item -ItemType Directory -Force -Path $data
}

for($i = 0; $i -lt $list.length; ++$i){
        Invoke-WebRequest -OutFile $data\raw -Uri $list[$i]
        Get-Content $data\raw | Out-File -Encoding ASCII $data\$i.txt
        Remove-Item $data\raw
}

if(Test-Path -PathType leaf .\spy){
        Remove-Item .\spy
}

.\merge_host.exe .\spy $data\0.txt

Remove-Item $data\*.txt
