package com.demo.springmvc.controller;

import com.demo.springmvc.entity.User;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;

@Controller
@RequestMapping(value = "/user2")
public class ModelController {

    /*
        被ModelAttribute标注的方法中，在该Controller被调用前,先执行
        @ModelAttribute方法
     */
    @ModelAttribute("user")
    public User userModel(@RequestParam("username")String username,
                          @RequestParam("password")String password){
        User user = new User();
        user.setUsername(username);
        user.setPassword(password);

        return user;
    }

    @RequestMapping("/login")
    public String login(@ModelAttribute("user")User user){
        return "success";
    }
}
