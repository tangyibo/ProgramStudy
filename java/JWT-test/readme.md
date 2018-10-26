JWT token
==============

在使用之前先解释下上面参数的意思：

=============    ===================

   名称	             解释

-------------    ------------------

iss (issuer)	  issuer 请求实体，可以是发起请求的用户的信息，也可是jwt的签发者
-------------    -------------------------------------
sub (Subject)	  设置主题，类似于发邮件时的主题
-------------    -----------------------------------
aud (audience)	  接收jwt的一方
--------------   ----------------------------------
exp (expire)	  token过期时间
-------------    ---------------------------------
nbf (not before)  前时间在nbf设定时间之前，该token无法使用
--------------   ---------------------------------
iat (issued at)	  token创建时间
-------------    ---------------------------------
jti (JWT ID)	  对当前token设置唯一标示

=======================================================

document
===============
https://segmentfault.com/a/1190000009981879