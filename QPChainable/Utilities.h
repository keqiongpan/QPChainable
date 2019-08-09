//
//  Utilities.h
//  QPChainable
//
//  Created by keqiongpan@163.com on 2019/8/1.
//  Copyright © 2019 Qiongpan Ke. All rights reserved.
//


#pragma mark - QPChainableArray(objecttype, __VA_ARGS__)

/**
 *  将宏函数的`...'形参（实参为{object1, object2, ...}的形式）转换为NSArray类型
 *  的对象数组。通常用于定义支持链式表达式的宏函数，方便在使用时支持花括号`{'后
 *  的换行对齐，同时统一数组字面量值的提供方式。
 *
 *  @example 使用示例，如下：
 *
 *      #define members(...) \
 *              membersFromArray(QPChainableArray(NSString *, __VA_ARGS__))
 *      - QP_CHAINABLE_DECLARE(members, (void *_FOR_CODE_HINT_ONLY_));
 *      - QP_CHAINABLE_DECLARE(membersFromArray, (NSArray<NSString *> *));
 *
 *      unsc.permanent.members({
 *          @"中国",
 *          @"美国",
 *          @"英国",
 *          @"法国",
 *          @"俄罗斯"
 *      })
 */
#ifndef QPChainableArray
#define QPChainableArray(objecttype, ... /* __VA_ARGS__ */) \
/**/    ({ \
/**/        objecttype _objects[] = __VA_ARGS__; \
/**/        NSUInteger _count = sizeof(_objects) / sizeof(_objects[0]); \
/**/        [NSArray<objecttype> arrayWithObjects:_objects count:_count]; \
/**/    })
#endif


#pragma mark - QPChainableDictionary(keytype, valuetype, __VA_ARGS__)

/**
 *  将宏函数的`...'形参（实参为 { key1:value1, key2:value2, ... } 的形式）转换为
 *  NSDictionary类型的字典组。通常用于定义支持链式表达式的宏函数，方便在使用时支
 *  持花括号`{'后的换行对齐，同时统一字典组字面量值的提供方式。
 *
 *  @example 使用示例，如下：
 *
 *      #define members(...) \
 *              membersFromDictionary(QPChainableDictionary(NSString *, NSString *, __VA_ARGS__))
 *      - QP_CHAINABLE_DECLARE(members, (void *_FOR_CODE_HINT_ONLY_));
 *      - QP_CHAINABLE_DECLARE(membersFromDictionary, (NSDictionary<NSString *, NSString *> *));
 *
 *      unsc.permanent.members({
 *          @"CN" : @"中国",
 *          @"US" : @"美国",
 *          @"UK" : @"英国",
 *          @"FR" : @"法国",
 *          @"RU" : @"俄罗斯"
 *      })
 */
#ifndef QPChainableDictionary
#define QPChainableDictionary(keytype, valuetype, ... /* __VA_ARGS__ */) \
/**/    ((NSDictionary<keytype, valuetype> *)@__VA_ARGS__)
#endif
