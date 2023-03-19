import sys
import requests


url = "https://aa2aa.pythonanywhere.com/SendPackage"

payload={'BlockPackage': sys.argv[1]}
files=[

]
headers = {}

response = requests.request("POST", url, headers=headers, data=payload, files=files)

print(response.text)