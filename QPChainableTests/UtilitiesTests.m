//
//  UtilitiesTests.m
//  QPChainable
//
//  Created by keqiongpan@163.com on 2020/1/13.
//  Copyright © 2020 Qiongpan Ke. All rights reserved.
//

#import <XCTest/XCTest.h>
#import <QPChainable/QPChainable.h>


#pragma mark - QPInternationalOrganization

@interface QPInternationalOrganization : NSObject

@property (nonatomic, copy) NSArray<NSString *> *countryMembers;
@property (nonatomic, copy) NSDictionary<NSString *, NSString *> *countryCodes;
@property (nonatomic, copy) NSString *(^organizationSerialNoBlock)(void);

#define members(...) \
        membersFromArray(QPChainableArray(NSString *, __VA_ARGS__))
- QP_CHAINABLE_DECLARE(members, (void *_FOR_CODE_HINT_ONLY_));
- QP_CHAINABLE_DECLARE(membersFromArray, (NSArray<NSString *> *));

#define codes(...) \
        codesFromDictionary(QPChainableDictionary(NSString *, NSString *, __VA_ARGS__))
- QP_CHAINABLE_DECLARE(codes, (void *_FOR_CODE_HINT_ONLY_));
- QP_CHAINABLE_DECLARE(codesFromDictionary, (NSDictionary<NSString *, NSString *> *));

#define serialNo(...) \
        serialNo(QPChainableEvaluation(NSString *, self, __VA_ARGS__))
- QP_CHAINABLE_DECLARE(serialNo, (NSString *(^)(void)));

@end

@implementation QPInternationalOrganization

#pragma mark members({name, ...})
- QP_CHAINABLE_IMPLEMENTATION(members, (void *_FOR_CODE_HINT_ONLY_), ());
- QP_CHAINABLE_IMPLEMENTATION(membersFromArray, (NSArray<NSString *> *members), ({
    self.countryMembers = members;
}));

#pragma mark codes({code:name, ...})
- QP_CHAINABLE_IMPLEMENTATION(codes, (void *_FOR_CODE_HINT_ONLY_), ());
- QP_CHAINABLE_IMPLEMENTATION(codesFromDictionary, (NSDictionary<NSString *, NSString *> *codes), ({
    self.countryCodes = codes;
}));

#pragma mark serialNo({expression})
- QP_CHAINABLE_IMPLEMENTATION(serialNo, (NSString *(^serialNoBlock)(void)), ({
    self.organizationSerialNoBlock = serialNoBlock;
}));

@end


#pragma mark - UtilitiesTests

@interface UtilitiesTests : XCTestCase

@end

@implementation UtilitiesTests

- (void)testArray {
    QPInternationalOrganization *io = QPInternationalOrganization.new;
    XCTAssert(0 == io.countryMembers.count, "初始化时成员个数应为0");
    io.members({
        @"中国",
        @"美国",
        @"英国",
        @"法国",
        @"俄罗斯"
    });
    XCTAssert([(@[
        @"中国",
        @"美国",
        @"英国",
        @"法国",
        @"俄罗斯"
    ]) isEqualToArray:io.countryMembers], "设置数组后字面量与实际值应一致。");
}

- (void)testDictionary {
    QPInternationalOrganization *io = QPInternationalOrganization.new;
    XCTAssert(0 == io.countryCodes.count, "初始化时成员个数应为0");
    io.codes({
        @"CN" : @"中国",
        @"US" : @"美国",
        @"UK" : @"英国",
        @"FR" : @"法国",
        @"RU" : @"俄罗斯"
    });
    XCTAssert([(@{
        @"CN" : @"中国",
        @"US" : @"美国",
        @"UK" : @"英国",
        @"FR" : @"法国",
        @"RU" : @"俄罗斯"
    }) isEqualToDictionary:io.countryCodes], "设置字典后字面量与实际值应一致。");
}

- (void)testStaticEvaluation {
    QPInternationalOrganization *io = QPInternationalOrganization.new;
    XCTAssert(nil == io.organizationSerialNoBlock, "初始化时计算表达式应为空。");
    io.serialNo(@"unsc-unique-id");
    XCTAssert(nil != io.organizationSerialNoBlock, "设置后计算表达式应不为空。");
    XCTAssert([@"unsc-unique-id" isEqualToString:io.organizationSerialNoBlock()], "静态计算表达式的结果应与设置的值一致。");
    XCTAssert([io.organizationSerialNoBlock() isEqualToString:io.organizationSerialNoBlock()], "静态计算表达式的多次计算结果均应一致。");
}

- (void)testDynamicEvaluation {
    QPInternationalOrganization *io = QPInternationalOrganization.new;
    XCTAssert(nil == io.organizationSerialNoBlock, "初始化时计算表达式应为空。");
    io.serialNo({[NSString stringWithFormat:@"unsc-unique-id-%d", (int)random()];});
    XCTAssert(nil != io.organizationSerialNoBlock, "设置后计算表达式应不为空。");
    XCTAssert([io.organizationSerialNoBlock() hasPrefix:@"unsc-unique-id"], "动态计算表达式的结果由设置的表达式动态计算获得。");
    XCTAssertFalse([io.organizationSerialNoBlock() isEqualToString:io.organizationSerialNoBlock()], "动态计算表达式的多次计算结果可以不一致。");
}

@end
