const output=document.querySelector('#result');
document.querySelectorAll('button[data-path]').forEach(button=>button.addEventListener('click',async()=>{
 const id=document.querySelector('#identity').value;
 try{const response=await fetch(button.dataset.path,{headers:id?{'X-Dev-User':id}:{}});const data=await response.json();output.textContent=`HTTP ${response.status}\n${JSON.stringify(data,null,2)}`;}
 catch(error){output.textContent=`连接失败：${error.message}`;}
}));
