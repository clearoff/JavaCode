package com.demo.springmvc.controller;

import com.demo.springmvc.entity.User;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.ui.ModelMap;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.servlet.ModelAndView;

import java.io.IOException;
import java.io.Writer;

/*
    注意:如果在类外使用RequestMapping注解
    则需要url位:http://ip:port/user/register才能匹配上
    同时还需要注意修改jsp文件中相应的action.
 */
@Controller
@RequestMapping(value="/user")
public class UserController {

    @RequestMapping(value="/register")
    public ModelAndView Register(@ModelAttribute("form")User user){

        //使用ModelMap
//        ModelMap model = new ModelMap();
//        model.addAttribute("user",user);

        //使用ModelAndView
        ModelAndView mv = new ModelAndView();
        mv.addObject("user",user);
        //如果在springmvc-servlet核心配置文件中配置了视图解析器,则在controller中视图名称可以简写
        //否则就需要写全称
//        mv.setViewName("/WEB-INF/views/success.jsp");
        mv.setViewName("success ");
        return mv;
    }

    @RequestMapping(value = "/login")
    public String login(@RequestParam("username")String username,
                              @RequestParam("password") String password){
        if(username.equalsIgnoreCase("leap") &&
                password.equals("123456")){
            return "Hello,World!";
        }
        else{
            return "hehe";
        }
    }

    //ResponseBody
    @RequestMapping(value="/something",method= RequestMethod.PUT)
    public void doSomething(@RequestBody String body, Writer writer) throws IOException{
        writer.write(body);
    }
}
