Something For Vim And Markdown
------
first of all,try to install a tool to show markdown
\# or run:
\# sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys BA300B7755AFCFAE

wget -qO - https://typora.io/linux/public-key.asc | sudo apt-key add -

\# add Typora's repository

sudo add-apt-repository 'deb https://typora.io/linux ./'

sudo apt-get update

\# install typora

sudo apt-get install typora

在文本下一行使用===或者---（数量不限），即可使文本成为最高阶标题和第二阶标题，
亦或是使用不同数量的符号#引导文本，生成h1~h6 的标题.such as
标题一(使用===)
===
标题二(use ---)
---
### 标题3(### 标题3）
#### 标题4(#### 标题4)
