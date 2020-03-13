import os
print(
'''
    ██╗   ██╗ ██████╗ ██╗      █████╗  ██████╗████████╗
    ╚██╗ ██╔╝██╔═══██╗██║     ██╔══██╗██╔════╝╚══██╔══╝
     ╚████╔╝ ██║   ██║██║     ███████║██║        ██║   
      ╚██╔╝  ██║   ██║██║     ██╔══██║██║        ██║   
       ██║   ╚██████╔╝███████╗██║  ██║╚██████╗   ██║   
       ╚═╝    ╚═════╝ ╚══════╝╚═╝  ╚═╝ ╚═════╝   ╚═╝ 

     █████╗ ██╗    ███████╗████████╗██╗   ██╗██████╗ ██╗ ██████╗
    ██╔══██╗██║    ██╔════╝╚══██╔══╝██║   ██║██╔══██╗██║██╔═══██╗
    ███████║██║    ███████╗   ██║   ██║   ██║██║  ██║██║██║   ██║
    ██╔══██║██║    ╚════██║   ██║   ██║   ██║██║  ██║██║██║   ██║
    ██║  ██║██║    ███████║   ██║   ╚██████╔╝██████╔╝██║╚██████╔╝
    ╚═╝  ╚═╝╚═╝    ╚══════╝   ╚═╝    ╚═════╝ ╚═════╝ ╚═╝ ╚═════╝
'''
)

DOWNLOAD=True #下载设置True 安装设置Fals
if DOWNLOAD:
	os.system('pip download bs4 lxml xlwt xlrd seaborn -i https://pypi.tuna.tsinghua.edu.cn/simple -d pypi')
	os.system('pip download torch==1.4.0+cu92 torchvison==0.5.0+cu92 -f https://download.pytorch.org/whl/torch_stable.html -d pypi')
else:
	os.system('pip install bs4 lxml xlwt xlrd seaborn torch torchvison --no-index -f ./pypi')