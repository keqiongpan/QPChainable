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


#pragma mark - QPChainableEvaluation(resulttype, self, __VA_ARGS__)

/**
 *  将宏函数的`...'形参（实参为 expression 或 { expression; } 的形式）转换为合适
 *  的表达式计算的block返回。`...'形参有两种表示方式，直接书写的expression表示立
 *  即计算表达式的结果，并作为后续每次调用该block时的固定返回结果；而使用花括号
 *  `{}'括起的expression表示后续每次调用该block时都重新计算表达式的结果返回。需
 *  要注意的是，使用花括号`{}'括起来时，相当于行内代码块`({...})'，可以书写多行
 *  语句，最后一行为返回结果，并需要添加分号`;'作为语句行结尾。
 *
 *  @note 表达式中出现的self将会被指向selfvalue参数的值，并且返回的block弱引用该
 *      参数的值，所以即使不作特殊处理，在表达式中使用self也不会产生循环引用。
 *
 *  @example 使用示例，如下：
 *
 *      #define appellation(...) \
 *              appellation(QPChainableEvaluation(NSString *, self, __VA_ARGS__))
 *      - QP_CHAINABLE_DECLARE(appellation, (NSString *(^)(void)));
 *
 *      // 一次性设置一个值。
 *      person.appellation([self.lastName stringByAppendingString:@"老师"])
 *
 *      // 每次使用时重新计算结果。
 *      person.appellation({
 *          NSString *title = self.male ? @"先生" : @"女士";
 *          [self.lastName stringByAppendingString:title];
 *      })
 */
#ifndef QPChainableEvaluation
#define QPChainableEvaluation(resulttype, selfvalue, ...) \
/**/    ({ \
/**/        NSString *_expression = @#__VA_ARGS__; \
/**/        resulttype (^_inputBlock)(__typeof(selfvalue) self); \
/**/        resulttype (^_outputBlock)(void); \
/**/        _inputBlock = ^resulttype (__typeof(selfvalue) self) { return (__VA_ARGS__); }; \
/**/        if ([_expression hasPrefix:@"{"] && [_expression hasSuffix:@"}"]) { \
/**/            __weak __typeof(selfvalue) _weakSelf = (selfvalue); \
/**/            _outputBlock = ^resulttype (void) { return _inputBlock(_weakSelf); }; \
/**/        } \
/**/        else { \
/**/            resulttype _resultValue = _inputBlock(selfvalue); \
/**/            _outputBlock = ^resulttype (void) { return _resultValue; }; \
/**/        } \
/**/        _outputBlock; \
/**/    })
#endif
