# ssh 克隆仓库遇到 Permission denied (publickey).

1.  调试看信息：    

github:
```
 ssh -vt git@github.com  
```
gerrit:
```
 ssh -vv -p 29418 ut003691@gerrit.uniontech.com
```
 信息里出现：` send_pubkey_test: no mutual signature algorithm  `  
   由于各种安全漏洞，RSA SHA-1 哈希算法正迅速在操作系统和 SSH 客户端中被弃用，其中许多技术现在完全拒绝使用该算法。注意，默认情况下 Bitbucket 提供对 ECDSA 和 ED25519 算法的支持。 使用这些算法生成的密钥不受 RSA 弃用的影响。

   Resolutions:  
   
   1. Workaround for re-enabling RSA
   
   In order to re-enable ssh-rsa support, inserting the following line into the affected SSH client's config file can re-enable this algorithm:
   `PubkeyAcceptedKeyTypes +ssh-rsa`

   2. Resolution using ECDSA and ED25519 algorithms  
   
```
   ssh-keygen -t ed25519 -C "your_email@example.com"
```
more：

Verify that you have a private key generated and loaded into SSH.

```
ssh-add -l -E sha256
```

   [reference url]( https://confluence.atlassian.com/bitbucketserverkb/ssh-rsa-key-rejected-with-message-no-mutual-signature-algorithm-1026057701.html)

# github ssh 网络问题

1. 安装gh （github-cli)

```sh
gh auth login
```

登录认证，选择协议http

