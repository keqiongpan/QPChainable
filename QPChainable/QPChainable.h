//
//  QPChainable.h
//  QPChainable
//
//  Created by keqiongpan@163.com on 2019/6/5.
//  Copyright © 2019 Qiongpan Ke. All rights reserved.
//

#import <Foundation/Foundation.h>

//! Project version number for QPChainable.
FOUNDATION_EXPORT double QPChainableVersionNumber;

//! Project version string for QPChainable.
FOUNDATION_EXPORT const unsigned char QPChainableVersionString[];

// In this header, you should import all the public headers of your framework using statements like #import <QPChainable/PublicHeader.h>


#pragma mark - QPChainableDeclare(name, arguments[, returntype])

/**
 *  在类声明体中添加支持链式表达式的成员函数。
 *
 *  @param name         函数名，表示链式表达式中的一个操作。
 *  @param arguments    函数的形参列表，需要使用`()'括起来。
 *                      如果没有参数，可使用`(void)'来表示。
 *  @param returntype   函数的返回值类型，可选参数，默认为intancetype。
 *
 *  @example 使用示例，如下：
 *      \@interface QPPacketMaker : NSObject
 *      - QPChainableDeclare(length, (void), NSUInteger);
 *      - QPChainableDeclare(append, (QPMultipartData *data));
 *      - QPChainableDeclare(send, (void));
 *      \@end
 *
 *  @note 注意， returntype 一般为当前链式表达式函数声明所在类的实例类型。如果要
 *      想使子类实现的链式表达式函数与父类实现的链式表达式函数混用，可以将子类与
 *      父类均改造为泛型类，并将面向最终使用者的实例类型通过泛型参数传递给子类和
 *      父类，子类和父类均使用该实例类型作为链式表达式函数的返回值类型，在函数实
 *      现时将self返回即可。然后为子类或父类实现面向最终使用者的、非泛型的、无成
 *      员（方法、属性、实例变量等）的、不可被继承的代理子类（Proxy Subclass），
 *      并将其基类的泛型参数声明为该代理子类的实例类型。这样，原来子类与父类实现
 *      的链式表达式函数就可以进行混用了，并且这些链式表达式函数也能被后代子类继
 *      续继承。但需要注意到，这样处理之后，在类的继承体系上会多出一些无成员的、
 *      不可被继承的代理子类，但由于没有额外的成员（方法、属性、实例变量等），在
 *      理论上是可以将父类实例指针强制转换为代理子类实例指针使用的。
 *
 *      详细示例如下：
 *
 *      \@interface QPConcreteBase<instancetype> : NSObject
 *      - QPChainableDeclare(setValue, (id value));
 *      \@end
 *
 *      \@interface QPConcreteChild<instancetype> : QPConcreteBase<instancetype>
 *      - QPChainableDeclare(dump, (void));
 *      \@end
 *
 *      \@class QPChild;
 *      \@interface QPChild : QPConcreteChild<QPChild *>
 *      \@end
 *
 *      QPChild.new.setValue(...).dump();
 */
#ifndef QPChainableDeclare

#define QPChainableDeclare(... /* name, arguments[, returntype] */) \
/**/    _QPChainableDecalre_internal(__VA_ARGS__, N3, N2, N1)

#define _QPChainableDecalre_internal(v1, v2, v3, suffix, ...) \
/**/    _QPChainableDecalre_supports_only_2_or_3_arguments_##suffix(v1, v2, v3)

#define _QPChainableDecalre_supports_only_2_or_3_arguments_N2(name, arguments, ...) \
/**/    (instancetype (^) arguments)name

#define _QPChainableDecalre_supports_only_2_or_3_arguments_N3(name, arguments, returntype, ...) \
/**/    (returntype (^) arguments)name

#endif


#pragma mark - QPChainableImplementation(name, arguments[, returntype], body)

/**
 *  在类实现体中添加支持链式表达式的成员函数。
 *
 *  @param name         函数名，表示链式表达式中的一个操作。
 *  @param arguments    函数的形参列表，需要使用`()'括起来。
 *                      如果没有参数，可使用`(void)'来表示。
 *  @param returntype   函数的返回值类型，可选参数，默认为intancetype。
 *  @param body         函数的实现体，使用`QPChainableBody({})'括起来。
 *
 *  @example 使用示例，如下：
 *      \@implementation QPPacketMaker
 *      - QPChainableImplementation(length, (void), NSUInteger, QPChainableBody({
 *          return self.data.length;
 *      }));
 *      \@end
 */
#ifndef QPChainableImplementation

#define QPChainableImplementation(... /* name, arguments[, returntype], body */) \
/**/    _QPChainableImplementation_internal(__VA_ARGS__, N4, N3, N2, N1)

#define _QPChainableImplementation_internal(v1, v2, v3, v4, suffix, ...) \
/**/    _QPChainableImplementation_supports_only_3_or_4_arguments_##suffix(v1, v2, v3, v4)

#define _QPChainableImplementation_supports_only_3_or_4_arguments_N3(name, arguments, body, ...) \
/**/    (id (^) arguments)name { \
/**/        return ^id arguments { \
/**/            do { \
/**/                _You_need_wrap_the_implementation_body_with_QPChainableBody_##body; \
/**/            } while (0); \
/**/            return self; \
/**/        }; \
/**/    }

#define _QPChainableImplementation_supports_only_3_or_4_arguments_N4(name, arguments, returntype, body, ...) \
/**/    (returntype (^) arguments)name { \
/**/        return ^returntype arguments { \
/**/            do { \
/**/                _You_need_wrap_the_implementation_body_with_QPChainableBody_##body; \
/**/            } while (0); \
/**/        }; \
/**/    }

#endif


#pragma mark - QPChainableBody({ ... do something here ... })

/**
 *  与QPChainableImplementation搭配使用，将函数体括起来。主要是方便预编译程序将
 *  其识别为一个整体的参数值传递给宏函数内部，避免因代码中的英文逗号“,”被识别
 *  为宏函数的多个参数值。
 *
 *  @param ...  可以是单条语句，或用花括号`{}'括起来的语句块。
 *
 *  @see QPChainableImplementation
 */
#ifndef QPChainableBody

#define QPChainableBody(...) \
/**/    _QPChainableBody_internal(__VA_ARGS__)

#define _You_need_wrap_the_implementation_body_with_QPChainableBody__QPChainableBody_internal(...) \
/**/    __VA_ARGS__

#endif


#pragma mark - QPChainableProxySubclassDeclare(subclass, superclass)

/**
 *  声明代理子类（Proxy Subclass）。代理子类主要是为了去掉父类中的泛型参数，并将
 *  自身的实例类型传递给父类及其祖先类使用，使得从这些类中继承的链式表达式函数都
 *  能返回这个代理子类的实例类型，继而支持混用。所以，代理子类主要是面向最终使用
 *  者的、非泛型的、无成员（方法、属性、实例变量等）的和不可被继承的类。
 *
 *  @see QPChainableDeclare, QPChainableProxySubclassImplementation
 */
#ifndef QPChainableProxySubclassDeclare
#define QPChainableProxySubclassDeclare(subclass, superclass) \
/**/    @class subclass; \
/**/    @interface superclass (QPChainable) \
/**/    - (subclass *)make; \
/**/    @end \
/**/    @interface subclass : superclass<subclass *> \
/**/    @end
#endif


#pragma mark - QPChainableProxySubclassImplementation(subclass, superclass)

/**
 *  实现代理子类（Proxy Subclass）。
 *
 *  @see QPChainableDeclare, QPChainableProxySubclassDeclare
 */
#ifndef QPChainableProxySubclassImplementation
#define QPChainableProxySubclassImplementation(subclass, superclass) \
/**/    @implementation superclass (QPChainable) \
/**/    - (subclass *)make { \
/**/        return (subclass *)self; \
/**/    } \
/**/    @end \
/**/    @implementation subclass \
/**/    @end
#endif


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
 *      - QPChainableDeclare(members, (void *__FOR_CODE_HINT_ONLY__));
 *      - QPChainableDeclare(membersFromArray, (NSArray<NSString *> *));
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
 *      - QPChainableDeclare(members, (void *__FOR_CODE_HINT_ONLY__));
 *      - QPChainableDeclare(membersFromDictionary, (NSDictionary<NSString *, NSString *> *));
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
